#!/usr/bin/env python3

from argparse import ArgumentParser
from sys import argv, stderr
from php_git2 import generate_function_manifest
from php_git2 import collect_bindings
from php_git2 import collect_removed
from php_git2 import collect_testbed_units


def filter_prefix(prefix,bucket):
    repl = {}
    for fn in bucket:
        if fn[:len(prefix)] == prefix:
            repl[fn] = bucket[fn]
    return repl


def format_percentage(a,b):
    if b == 0:
        return "n/a"

    return "{:.2f}%".format(a / b * 100)


def main(args):
    arg_parser = ArgumentParser(description="Calculate php-git2 library coverage")
    arg_parser.add_argument("--summary",action="store_true",help="Show library coverage summary (default)")
    arg_parser.add_argument("--list-bindings",action="store_true",help="Show list of status of all possible library bindings")
    arg_parser.add_argument("--filter-prefix",help="Filter functions matching prefix")
    args_parsed = arg_parser.parse_args(args)

    manifest = generate_function_manifest()
    bindings = collect_bindings()
    removed = collect_removed()
    testbed = collect_testbed_units()

    if args_parsed.filter_prefix:
        filter_title = " (for {})".format(args_parsed.filter_prefix)
        manifest = filter_prefix(args_parsed.filter_prefix,manifest)
        bindings = filter_prefix(args_parsed.filter_prefix,bindings)
        removed = filter_prefix(args_parsed.filter_prefix,removed)
        testbed = filter_prefix(args_parsed.filter_prefix,testbed)
    else:
        filter_title = ""

    n_total = 0
    n_bindings = 0
    n_removed = 0
    n_testbed = 0

    repl = {}
    for fn in removed:
        if fn in manifest:
            n_removed += 1
            repl[fn] = removed[fn]
        else:
            print("Warning: removed function '{}' not found in headers".format(fn),file=stderr)
    removed = repl

    for fn in manifest:
        n_total += ( fn not in removed )
        n_bindings += ( fn in bindings )
        n_testbed += ( fn in testbed )

    n_extra = 0
    n_testbed_extra = 0

    for fn in bindings:
        n_extra += ( fn not in manifest )
        n_testbed_extra += ( fn not in manifest and fn in testbed )

    n_printed = 0

    if args_parsed.summary or (not args_parsed.list_bindings):
        n_printed += 1
        print("Summary of coverage{}:".format(filter_title))
        print("  Total git2 functions: {}".format(n_total))
        print("  Total removed functions: {}".format(n_removed))
        print("  Total function bindings: {}".format(n_bindings))
        print("  Total extra function bindings: {}".format(n_extra))
        print("  Total bindings tested: {}".format(n_testbed))
        print("  Binding coverage: {}".format(format_percentage(n_bindings,n_total)))
        print("  Test coverage: {}".format(format_percentage(n_testbed,n_bindings)))
        print("  Test coverage (extra): {}".format(format_percentage(n_testbed_extra,n_extra)))

    if args_parsed.list_bindings:
        if n_printed > 0:
            print("")
        n_printed += 1
        print("Library Bindings{}:".format(filter_title))
        print("    (Binding status: '.'=unimplemented,'+'=implemented,'-'=removed)")
        print("    (Testbed status: 'T'=testbed,'n'=not tested)")
        for fn in sorted(manifest.keys()):
            if fn in removed:
                status = "-"
            elif fn in bindings:
                status = "+"
            else:
                status = "."

            if fn in removed:
                tested = "-"
            elif fn in testbed:
                tested = "T"
            else:
                tested = "n"

            print("  {}{} {}".format(status,tested,fn))


main(argv[1:])
