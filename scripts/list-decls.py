#!/usr/bin/env python3

import clang.cindex
import sys
from typing import Iterator


def find_function_decls(node: clang.cindex.Cursor | None) -> Iterator[clang.cindex.Cursor]:
    if node is None:
        return

    if node.kind == clang.cindex.CursorKind.FUNCTION_DECL:
        yield node
    for child in node.get_children():
        yield from find_function_decls(child)


def find_functions(tu: clang.cindex.TranslationUnit) -> None:
    for node in find_function_decls(tu.cursor):
        params = ", ".join(f"{x.canonical.type.spelling} {x.displayname}"
                           for x in node.get_arguments()
                           if x is not None)
        print(f"{node.result_type.spelling} {node.spelling}({params});")


defines = {
    "Procedure": "void",
    "Visible": "",
    "Hidden": "static",
    "Forward": "",
    "Yes": "true",
    "No": "false",
}


extra_flags = [
    "-Isrc",
    "-Isrc/unix/",
    "-Isrc/bhdrs/",
    "-Isrc/ihdrs/",
    "-Isrc/ehdrs/",
    "-Isrc/btr/",
]


def main() -> None:
    extra_defines = [f"-D{k}={v}" for k, v in defines.items()]
    filename = sys.argv[1]
    index = clang.cindex.Index.create()
    tu = index.parse(filename, args=["--std=gnu90", *extra_flags, *extra_defines])
    find_functions(tu)


if __name__ == "__main__":
    main()
