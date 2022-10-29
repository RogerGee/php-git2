# php_git2/__init__.py

import os
import re
import sys


PHP_GIT2_REGEX_CACHE = {}


def find_install_base():
    """
    Finds the base directory of the configured git2 library.
    """

    if "GIT2_DIR" in os.environ:
        return os.environ["GIT2_DIR"]

    return "/usr/local"


def search_dir_recursive(dir,callback,depth = -1):
    def search_recursive(iter,d):
        for entry in iter:
            if entry.is_file():
                f = open(entry.path)

                try:
                    contents = f.read()
                except UnicodeDecodeError:
                    print("Skipping file '{}' due to binary content".format(entry.path),file=sys.stderr)
                    f.close()
                    continue

                callback(entry.name,entry.path,contents)
                f.close()
            elif entry.is_dir() and d != 0:
                with os.scandir(entry.path) as it:
                    search_recursive(it,d-1)

    with os.scandir(dir) as iter:
        search_recursive(iter,depth)


def search_git2_include_files(callback):
    base = find_install_base()
    includePath = os.path.join(base,"include","git2")
    search_dir_recursive(includePath,callback)


def get_regex(key):
    global PHP_GIT2_REGEX_CACHE

    if key in PHP_GIT2_REGEX_CACHE:
        return PHP_GIT2_REGEX_CACHE[key]

    if key == "parse_function_block":
        regex = re.compile("GIT_EXTERN\([^)]+\)[^)]+\);")
    elif key == "parse_function":
        regex = re.compile("GIT_EXTERN\(([^)]+)\)\s*(git_[^ \(]+)\(([^\)]+)\);")
    elif key == "split_function_arguments":
        regex = re.compile("\s*,\s*")
    elif key == "parse_binding_macro":
        regex = re.compile("PHP_GIT2_FE\(([^)]+)\)")
    elif key == "php_file":
        regex = re.compile("^(.+)\.php$")
    elif key == "php_unit_class":
        regex = re.compile("/\*\*(.+?)\*/\s*final\s*class",re.DOTALL)
    elif key == "php_unit_test":
        regex = re.compile("/\*\*(.+?)\*/\s*(?:public)?\s*function\s*(\S+?)\s*\(([^)]*)\)",re.DOTALL)
    elif key == "docblock":
        regex = re.compile("@(\S+)(?: (.+))?")
    else:
        raise Exception("get_regex({}) is not defined".format(key))

    PHP_GIT2_REGEX_CACHE[key] = regex
    return regex


def parse_header_function(block):
    regex = get_regex("parse_function")

    result = regex.match(block)
    if result is None:
        return None

    arguments = [s.strip() for s in result.group(3).split(",")]

    return {
        "name": result.group(2),
        "returnType": result.group(1),
        "arguments": tuple(arguments)
    }


def generate_function_manifest():
    regex = get_regex("parse_function_block")

    # Generate a manifest of all external functions found in the header files.
    manifest = {}
    def callback(name,path,contents):
        p = 0
        while True:
            result = regex.search(contents,p)
            if result is None:
                break

            block = result.group()
            funcInfo = parse_header_function(block)
            if funcInfo is not None:
                manifest[funcInfo["name"]] = funcInfo

            p = result.end()

    search_git2_include_files(callback)

    return manifest


def collect_bindings():
    basepath = "."
    regex = get_regex("parse_binding_macro")

    results = {}
    def collect_fn(name,path,contents):
        if name[-2:] != ".h":
            return

        split_regex = get_regex("split_function_arguments")

        for macro in regex.findall(contents):
            args = split_regex.split(macro)
            assert len(args) == 3
            results[args[0]] = {
                "name": args[0],
                "c-func": args[1],
                "arginfo": args[2]
            }

    search_dir_recursive(basepath,collect_fn,depth=0)

    return results


def collect_removed():
    basepath = "./testbed/src/Test"
    regex = get_regex("php_unit_class")

    def parse_tags(docblock):
        docblock_parser = get_regex("docblock")
        tags = {
            "phpGitRemoved": []
        }
        for tag in docblock_parser.findall(docblock):
            if tag[0] in tags:
                tags[tag[0]].append(tag[1])
            else:
                tags[tag[0]] = [tag[1]]

        return tags

    results = {}
    def collect_fn(name,path,contents):
        m = get_regex("php_file").match(name)
        if not m:
            return

        m = regex.search(contents)
        if not m:
            return
        tags = parse_tags(m[1])

        for fn in tags["phpGitRemoved"]:
            results[fn] = True

    search_dir_recursive(basepath,collect_fn)

    return results


def collect_testbed_units():
    basepath = "./testbed/src/Test"
    regex = get_regex("php_unit_test")

    def parse_unit(docblock,name,arglist):
        docblock_parser = get_regex("docblock")
        unit = {
            "name": name,
            "arglist": arglist,
            "tags": {
                "phpGitTest": []
            }
        }
        for tag in docblock_parser.findall(docblock):
            if tag[0] in unit["tags"]:
                unit["tags"][tag[0]].append(tag[1])
            else:
                unit["tags"][tag[0]] = [tag[1]]

        return unit

    results = {}
    def collect_fn(name,path,contents):
        m = get_regex("php_file").match(name)
        if not m:
            return
        class_name = m[1]
        units = [parse_unit(*x) for x in regex.findall(contents)]

        for unit in units:
            for fn in unit["tags"]["phpGitTest"]:
                if fn not in results:
                    results[fn] = []
                results[fn].append("{}::{}".format(class_name,unit["name"]))

    search_dir_recursive(basepath,collect_fn)

    return results
