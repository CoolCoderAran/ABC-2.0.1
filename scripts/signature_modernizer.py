#!/usr/bin/env python3

"""
This code has been generated using Claude AI
"""

"""
modernize_c.py

Converts K&R / C90 function signatures to modern C23 style using pattern
matching. Handles both multiline K&R style (decls on separate lines) and
inline K&R style (decls on same line as closing paren).

Processes all .c files under src/ (or a given directory) in place.

Usage:
    python modernize_c.py              # processes ./src/
    python modernize_c.py path/to/src  # custom directory
"""

import re
import sys
import os


def parse_param_decls(decl_block: str, param_names: list[str]) -> dict[str, str]:
    """
    Parse a K&R parameter declaration block like:
        value lwb, upb;  int (*fn)();  value *pl;

    We use the known param names to correctly identify where the type ends and
    the names begin in each semicolon-separated declaration.

    Returns a dict of {param_name: full_typed_decl}.
    """
    known = set(param_names)
    params: dict[str, str] = {}

    for decl in decl_block.split(";"):
        decl = decl.strip()
        if not decl:
            continue

        # Function pointer: type (*name)(args)
        fp = re.match(r"^([\w\s\*]+)\(\s*\*\s*(\w+)\s*\)\s*\(([^)]*)\)\s*$", decl)
        if fp:
            ret_type = fp.group(1).strip()
            pname    = fp.group(2).strip()
            args     = fp.group(3).strip() or "int"
            params[pname] = f"{ret_type} (*{pname})({args})"
            continue

        # Find all known param names mentioned in this decl (with optional leading *)
        # e.g. "value lwb, upb" -> names_found = ['lwb', 'upb']
        # e.g. "value *pl"      -> names_found = ['pl']
        names_found = [n for n in param_names if re.search(rf"\b{re.escape(n)}\b", decl)]

        if not names_found:
            continue

        # `register` was removed from C23 entirely — strip it from param decls.
        # All other qualifiers (const, volatile, restrict, unsigned, long, etc.) are kept.
        decl = re.sub(r"\bregister\b\s*", "", decl)

        # The base type is everything before the first name token (strip trailing comma/stars/space)
        first_name = names_found[0]
        type_match = re.match(rf"^(.*?)\s*\**\s*\b{re.escape(first_name)}\b", decl)
        base_type  = type_match.group(1).rstrip(", *").strip() if type_match else "/* unknown */"

        for pname in names_found:
            # Capture ALL stars immediately preceding the param name (e.g. **area -> **)
            # \s* between stars and name is intentional in source but must be collapsed
            star_match = re.search(rf"(\*+)\s*\b{re.escape(pname)}\b", decl)
            stars = star_match.group(1) if star_match else ""
            params[pname] = f"{base_type} {stars}{pname}".strip()

    return params


def modernize_function(match: re.Match) -> str:
    prefix     = match.group(1) or ""
    fname      = match.group(2)
    param_list = match.group(3)
    decl_block = match.group(4)
    brace      = match.group(5)

    # Implicit-int: bare name at start of line with no return type
    if not prefix.strip():
        prefix = "int "

    param_names = [p.strip() for p in param_list.split(",") if p.strip()]
    param_types = parse_param_decls(decl_block, param_names)
    new_params  = [param_types.get(p, p) for p in param_names]
    return f"{prefix}{fname}({', '.join(new_params)}) {brace}"


# Pattern 1: multiline K&R — decls on their own lines after the closing paren
MULTILINE_KNR = re.compile(
    r"([\w \t]+(?:\s*\*+\s*|\s+))?" # group 1: return type (words + optional stars), same line only
    r"(\w+)"                        # group 2: function name
    r"\s*\(\s*"
    r"([\w\s,]+?)"                  # group 3: bare param names
    r"\s*\)\s*\n"
    r"((?:[ \t]*[^{}\n]+;\n)+)"     # group 4: K&R decl lines
    r"(\{)",                        # group 5: opening brace
    re.MULTILINE,
)

# Pattern 2: inline K&R — decls on the same line as the closing paren
INLINE_KNR = re.compile(
    r"([\w \t]+(?:\s*\*+\s*|\s+))"  # group 1: return type (words + optional stars)
    r"(\w+)"                        # group 2: function name
    r"\s*\(\s*"
    r"([\w\s,]+?)"              # group 3: bare param names
    r"\s*\)\s*"
    r"((?:[^{}\n]+;\s*)+)"      # group 4: inline K&R decls
    r"(\{)",                    # group 5: opening brace
    re.MULTILINE,
)


def modernize_source(source: str) -> str:
    source = MULTILINE_KNR.sub(modernize_function, source)
    source = INLINE_KNR.sub(modernize_function, source)
    return source


def count_knr(source: str) -> int:
    return len(MULTILINE_KNR.findall(source)) + len(INLINE_KNR.findall(source))


def process_file(path: str) -> None:
    with open(path, "r", encoding="utf-8", errors="replace") as fh:
        original = fh.read()

    n = count_knr(original)
    modernised = modernize_source(original)

    if modernised == original:
        print(f"  unchanged: {path}")
        return

    with open(path, "w", encoding="utf-8") as fh:
        fh.write(modernised)

    print(f"  modernised: {path}  ({n} function(s) updated)")


def main() -> None:
    src_dir = sys.argv[1] if len(sys.argv) > 1 else "src"

    if not os.path.isdir(src_dir):
        print(f"Error: '{src_dir}' is not a directory.", file=sys.stderr)
        sys.exit(1)

    c_files = sorted(
        os.path.join(root, fname)
        for root, _dirs, files in os.walk(src_dir)
        for fname in files
        if fname.endswith(".c")
    )

    if not c_files:
        print(f"No .c files found under '{src_dir}'.")
        return

    print(f"Found {len(c_files)} .c file(s) under '{src_dir}'.\n")
    for path in c_files:
        process_file(path)


if __name__ == "__main__":
    main()
