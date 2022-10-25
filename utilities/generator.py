#!/usr/bin/env python
#
# generator.py
#
# Use this script to generate templates for a set of function bindings. Pipe the
# function names, one per line, to this program. It first generates the template
# declarations and then generates the function entry macro.

import re
import sys

def genFunc(func):
    print (("static constexpr auto ZIF_{} = zif_php_git2_function<\n" \
        + "    php_git2::func_wrapper<\n" \
        + "\n" \
        + "        >::func<{}>,\n" \
        + "    php_git2::local_pack<\n" \
        + "\n" \
        + "        >,\n" \
        + "    -1,\n" \
        + "    php_git2::sequence<>,\n" \
        + "    php_git2::sequence<>,\n" \
        + "    php_git2::sequence<>\n" \
        + "    >;\n").format(func.upper(),func))

def genFreeFunc(func,section):
    print (("static constexpr auto ZIF_{} = zif_php_git2_function_free<\n" \
           "    php_git2::local_pack<\n" \
           "        php_git2::php_resource_cleanup<php_git2::php_{}>\n" \
           "        >\n" \
           "    >;\n").format(func.upper(),section))

def genDefine(func,trailing = True):
    print("    PHP_GIT2_FE({},ZIF_{},NULL){}".format(func,func.upper()," \\" if trailing else ""))

def genDoc(func):
    print("{}()".format(func))

names = []
while True:
    line = sys.stdin.readline()
    if len(line) == 0:
        break

    func = re.search("[^\s]+",line)
    if func is None:
        print("error: bad input: '{}'".format(line))
    name = func.group(0)
    names.append(name)

    match = re.search("(.*)_free$",name)
    if match:
        genFreeFunc(name,match.group(1))
    else:
        genFunc(name)

print("#define GIT___FE \\")
for i in range(len(names)):
    genDefine(names[i], i + 1 < len(names))

print("")
print("-"*40)
print("[git_]")
print("-"*40)
for name in names:
    genDoc(name)
