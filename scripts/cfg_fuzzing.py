#!/usr/bin/env python3

import nltk
import random
import typing
import warnings
import os
import sys
import shutil
import string
from string import ascii_uppercase
from threading import Thread
from subprocess import Popen, PIPE, STDOUT
from nltk.parse.generate import generate as gen
from dataclasses import dataclass, field
from pathlib import Path

RUN_TIMEOUT = 1
TEST_AMOUNT = 1000

@dataclass
class StdData:
    stdout_data: str = ""
    stderr_data: str = ""

@dataclass
class HowToStruct:
    args: list[str] = field(default_factory=list)
    name: str = ""

grammar_string = """
    S -> X [1]
    X -> function-how-to [0.5] | predicate-how-to [0.5]

    put-body -> put-str [0.4] | put-num [0.4] | put-dict [0.1] | put-list[0.1]
    put-str -> "PUT" space str-expression " IN " "NEWSTR" new-line put-body [0.7] | "PUT" space str-expression " IN " "NEWSTR" [0.3]
    put-num -> "PUT" space num-expression " IN " "NEWNUM" new-line put-body [0.7] | "PUT" space num-expression " IN " "NEWNUM" [0.3]
    put-dict -> "PUT" space dict-expression " IN " "NEWDICT" new-line put-body [0.7] | "PUT" space dict-expression " IN " "NEWDICT" [0.3]
    put-list -> "PUT" space list-expression " IN " "NEWLIST" new-line put-body [0.7] | "PUT" space list-expression " IN " "NEWLIST" [0.3]


    predicate-how-to -> "HOW TO REPORT " examination-template ":" new-line-proper increase-indentation command-suite new-line examination-command [1]
    examination-template -> zeroadic-examination-template [0.3] | monadic-examination-template [0.35]| dyadic-examination-template [0.35]

    zeroadic-examination-template -> howto-name [1]
    monadic-examination-template -> howto-name template-operand [1]
    dyadic-examination-template -> template-operand howto-name template-operand [1]

    examination-command -> "SUCCEED" [0.2] | "FAIL" [0.2] | "REPORT" space test [0.6]

    function-how-to -> "HOW TO RETURN " formula-template ":" new-line-proper increase-indentation command-suite [1]
    formula-template -> zeroadic-formula-template [0.3] | monadic-formula-template [0.35] | dyadic-formula-template [0.35]

    zeroadic-formula-template -> howto-name [1]
    monadic-formula-template -> howto-name space template-operand [1]
    dyadic-formula-template -> template-operand space howto-name space template-operand [1]


    howto-name -> "HOWTONAME" [1]
    template-operand -> "NEWNAME" [1]


    command-suite -> simple-command [0.1] | command-sequence decrease-indentation [0.9]
    command-sequence -> command new-line command-sequence [0.6] | command empty [0.4]
    command -> simple-command [0.7] | control-command [0.3]
    simple-command -> put-str [0.5] | put-num [0.5]
    control-command -> if-command [0.5] | select-command [0.25] | while-command [0.25]
    single-command -> "PUT" space str-expression " IN " "NEWSTR" [0.4] | "PUT" space num-expression " IN " "NEWNUM" [0.4] | "PUT" space dict-expression " IN " "NEWDICT" [0.1] | "PUT" space list-expression " IN " "NEWLIST" [0.1]

    pass-command -> "PASS" [1]
    share-command -> "SHARE" space "STRNAME" [0.4] | "SHARE" space "NUMNAME" [0.4] | "SHARE" space "DICTNAME" [0.1] | "SHARE" space "LISTNAME" [0.1]


    if-command -> "IF " test ": " command-suite [1]
    select-command -> "SELECT " ": " alternative-suite [1]
    alternative-suite -> increase-indentation alternative-sequence decrease-indentation [1]
    alternative-sequence -> new-line single-alternative optional-alternative-sequence new-line else-alternative [1]
    single-alternative -> test ": " single-command [1]
    else-alternative -> "ELSE " ": " single-command [1]
    optional-alternative-sequence -> alternative-sequence [0.75] | empty [0.25]

    while-command -> "WHILE " test ": " command-suite [1]

    test -> tight-test [0.7] | conjunction [0.1] | disjunction [0.1] | negation [0.1]
    tight-test -> order-test [1]
    order-test -> order-test-num [0.4] | order-test-str [0.4] | order-test-dict [0.1] | order-test-list [0.1]
    right-test -> tight-test [1]
    order-test-num -> num-expression order-sign num-expression [0.9] | order-test-num order-sign num-expression [0.1]
    order-test-str -> str-expression order-sign str-expression [0.9] | order-test-str order-sign str-expression [0.1]
    order-test-dict -> dict-expression order-sign dict-expression [0.9] | order-test-dict order-sign dict-expression [0.1]
    order-test-list -> list-expression order-sign list-expression [0.9] | order-test-list order-sign list-expression [0.1]

    order-sign -> "<" [0.167] | "<=" [0.167] | "=" [0.167] | "<>" [0.165] | ">=" [0.167] | ">" [0.167]
    conjunction -> tight-test "AND" space right-test [0.9] | tight-test "AND" space conjunction [0.1]
    disjunction -> tight-test "OR" space right-test [0.9] | tight-test "OR" space disjunction [0.1]
    negation -> "NOT" space right-test [1]
    new-line -> new-line-proper indent [1]
    new-line-proper -> "\\n" [1]
    indent -> "   " [1]
    space -> " " [1]
    increase-indentation -> "   " [1]


    address -> list-expression "[" "SMALLINT" "]" [0.25] | list-expression "[" num-expression "]" [0.2] | list-expression "[" simple-expression "]" [0.05] | dict-expression "[" "SMALLINT" "]" [0.25] | dict-expression "[" num-expression "]" [0.2] | dict-expression "[" simple-expression "]" [0.05]


    simple-expression -> num-expression [0.4] | str-expression [0.4] | dict-expression [0.1] | list-expression [0.1]
    monadic-num-function -> "~" [0.2] | "+" [0.2] | "-" [0.2] | "*/" [0.2] | "/*" [0.2]
    dyadic-num-function -> "+" [0.2] | "-" [0.2] | "*" [0.2] | "/" [0.2] | "**" [0.2]

    dyadic-str-function -> str-expression "^" str-expression [0.2] | str-expression "^^" num-expression [0.1] | str-expression "^^" "SMALLINT" [0.1] | str-expression "@" num-expression [0.1] | str-expression "@" "SMALLINT" [0.1] | str-expression "|" num-expression [0.1] | str-expression "|" "SMALLINT" [0.1] | str-expression "@" num-expression "|" num-expression [0.1] | str-expression "@" "SMALLINT" "|" "SMALLINT" [0.1]
    monadic-str-function -> "#" [1]


    str-expression -> "(" str-std ")"[0.4] | "STRNAME" [0.4] | "(" str-formula ")" [0.2]
    num-expression -> "(" num-std ")" [0.35] | "NUMNAME" [0.3] | "(" num-formula ")" [0.35]
    dict-expression -> "("dict-std")" [0.1] | "DICTNAME" [0.9]
    list-expression -> "("list-std")" [0.1] | "LISTNAME" [0.9]


    num-formula -> monadic-num-function num-expression [0.45] | num-expression dyadic-num-function num-expression [0.45] | list-num-std [0.05] | "#" str-expression [0.05]
    str-formula -> dyadic-str-function [0.75] | list-str-std [0.15] | address [0.1]


    list-str-std -> " min " list-expression [0.2] | str-expression  " min " list-expression [0.2] | " max "  list-expression [0.2] | str-expression  " max " list-expression [0.2] | list-expression " item " num-expression [0.1] | list-expression " item " "SMALLINT" [0.1]
    list-num-std -> "# " list-expression [0.5] | str-expression  "# " list-expression [0.5]


    dict-std -> "split" space str-expression [1]

    list-std -> "keys" space dict-expression [1]


    str-std -> stripped-howto [0.167] | lower-howto [0.167] | upper-howto [0.167] | max-howto [0.167] | min-howto [0.167] | dyadic-str-function [0.165]
    lower-howto -> "lower" space str-expression [1]
    upper-howto -> "upper" space str-expression [1]
    max-howto -> "max" space str-expression [1]
    min-howto -> "min" space str-expression [1]
    stripped-howto -> "stripped" space str-expression [1]


    num-std -> root-howto [0.038] | exactly-howto [0.037] | n-root-howto [0.037] | abs-howto [0.038] | sign-howto [0.037] | sign-howto [0.037] | round-howto [0.038] | floor-howto [0.037] | ceiling-howto [0.037] | n-round-howto [0.037] | mod-howto [0.037] | e-const-howto [0.037] | exp-howto [0.038] | log-howto [0.037] | b-log-howto [0.037] | pi-const-howto [0.037] | sin-howto [0.037] | cos-howto [0.037] | tan-howto [0.037] | arctan-howto [0.037] | angle-howto [0.037] | radius-howto [0.037] | c-sin-howto [0.037] | c-cos-howto [0.037] | c-tan-howto [0.037] | c-arctan-howto [0.037] | c-angle-howto [0.037]
    root-howto -> "root" space num-expression [1]
    exactly-howto -> "exactly" space num-expression [1]
    n-root-howto -> num-expression space "root" space num-expression [0.5] | "SMALLINT" space "root" space num-expression [0.5]
    abs-howto -> "abs" space num-expression [1]
    sign-howto -> "sign" space num-expression [1]
    round-howto -> "round" space num-expression [1]
    floor-howto -> "floor" space num-expression [1]
    ceiling-howto -> "ceiling" space num-expression [1]
    n-round-howto -> num-expression space "round" space num-expression [0.5] | "SMALLINT" space "round" space num-expression [0.5]
    mod-howto -> num-expression space "mod" space num-expression [1]
    e-const-howto -> "e" [1]
    exp-howto -> "exp" space num-expression [1]
    log-howto -> "log" space num-expression [1]
    b-log-howto -> num-expression space "log" space num-expression [0.5] | "SMALLINT" space "log" space num-expression [0.5]
    pi-const-howto -> "pi" [1]
    sin-howto -> "sin" space num-expression [1]
    cos-howto -> "cos" space num-expression [1]
    tan-howto -> "tan" space num-expression [1]
    arctan-howto -> "arctan" space num-expression [1]
    angle-howto -> "angle" space "(" num-expression ", " num-expression ")" [1]
    radius-howto -> "radius" space "(" num-expression ", " num-expression ")" [1]
    c-sin-howto -> num-expression space "sin" space num-expression [0.5] | "SMALLINT" space "sin" space num-expression [0.5]
    c-cos-howto -> num-expression space "cos" space num-expression [0.5] | "SMALLINT" space "cos" space num-expression [0.5]
    c-tan-howto -> num-expression space "tan" space num-expression [0.5] | "SMALLINT" space "tan" space num-expression [0.5]
    c-arctan-howto -> num-expression space "arctan" space num-expression [0.5] | "SMALLINT" space "arctan" space num-expression [0.5]
    c-angle-howto -> num-expression space "angle" space "(" num-expression ", " num-expression ")" [0.5] | "SMALLINT" space "angle" space "(" num-expression ", " num-expression ")" [0.5]




    empty -> [1]
"""

# generate grammar and helper method definitions
grammar = nltk.PCFG.fromstring(grammar_string)
start = grammar.start()

def generate(symbol: nltk.grammar.Nonterminal, env: dict[str, list[typing.Any, int]], depth: int = 0):
    if depth >= 70:
        return [], env

    if isinstance(symbol, str):
        if symbol == "HOWTONAME":
            var = "howto" + str(random.randint(0, 100000)) + " "
            return [var], env

        if symbol == "STRNAME":
            if len(env.keys()) == 0:
                var = random_string()
                return [var], env
            else:
                strings = []
                for k in env.keys():
                    if env[k][1] == 0:
                        strings.append(k)
                if len(strings) == 0:
                    var = random_string()
                    return [var], env
                var = random.choice(strings)
                #warnings.warn("var acccessed", var)
                return [var], env

        if symbol == "NUMNAME":
            if len(env.keys()) == 0:
                var = str(random_number())
                return [var], env
            else:
                nums = []
                for k in env.keys():
                    if env[k][1] == 1:
                        nums.append(k)
                if len(nums) == 0:
                    var = str(random_number())
                    return [var], env
                var = random.choice(nums)
                #warnings.warn("var acccessed", var)
                return [var], env

        if symbol == "DICTNAME":
            if len(env.keys()) == 0:
                var = random_dict()
                return [var], env
            else:
                dicts = []
                for k in env.keys():
                    if env[k][1] == 2:
                        dicts.append(k)
                if len(dicts) == 0:
                    var = random_dict()
                    return [var], env
                var = random.choice(dicts)
                #warnings.warn("var acccessed", var)
                return [var], env

        if symbol == "LISTNAME":
            if len(env.keys()) == 0:
                var = random_list()
                return [var], env
            else:
                lists = []
                for k in env.keys():
                    if env[k][1] == 3:
                        lists.append(k)
                if len(lists) == 0:
                    var = random_list()
                    return [var], env
                var = random.choice(lists)
                #warnings.warn("var acccessed", var)
                return [var], env

        if symbol == "NEWSTR":
            var = "strvar" + str(random.randint(0, 100000)) + " "
            env[var] = ["nil", 0]
            return [var], env
        if symbol == "NEWNUM":
            var = "numvar" + str(random.randint(0, 100000)) + " "
            env[var] = ["nil", 1]
            return [var], env
        if symbol == "NEWDICT":
            var = "dictvar" + str(random.randint(0, 100000)) + " "
            env[var] = ["nil", 2]
            return [var], env
        if symbol == "NEWLIST":
            var = "listvar" + str(random.randint(0, 100000)) + " "
            env[var] = ["nil", 3]
            return [var], env
        if symbol == "NEWNAME":
            var = "randvar" + str(random.randint(0, 100000)) + " "
            env[var] = ["nil", random.choice([0, 1, 2, 3])]
            return [var], env
        if symbol == "SMALLINT":
            var = random_number(4)
            return [var], env


        return [symbol], env

    productions: list[nltk.grammar.Production] = grammar.productions(lhs=symbol)
    if not productions:
        return [], env
    chosen_production = random.choice(productions)

    result = []
    env2 = env
    for rhs_symbol in chosen_production.rhs():
        out, env2 = generate(rhs_symbol, env2, depth+1)
        result.extend(out)
    return result, env2

def random_string() -> str:
    return "\"" + ''.join(random.choice(string.ascii_letters + string.digits + ' ') for i in range(
            random.randint(1, 25)
        )) + "\""

def random_dict():
    length = random.randint(0, 10)
    out = "{"
    for i in range(length):
        out = out + "[" + str(random_number(4)) + "]: " + random_string()
        if i < length - 1:
            out = out + ";"
    out = out + "}"
    return out

def random_list():
    length = random.randint(0, 10)
    out = "{"
    for i in range(length):
        out = out + random_string()
        if i < length - 1:
            out = out + ";"
    out = out + "}"
    return out

def random_number(category = -1):
    if category == -1:
        category = random.choice([0, 1, 2, 3])
    match category:
        case 0:
            return random.randint(-sys.maxsize, sys.maxsize)
        case 1:
            return random.random()
        case 2:
            return random.uniform(-sys.maxsize, sys.maxsize)
        case 3:
            return random.uniform(sys.maxsize/1000, sys.maxsize)
        case 4:
            return random.randint(0, 20)


    return random.randint(-sys.maxsize, sys.maxsize)


def random_value():
    i = random.choice([0, 1, 2, 3])
    if i == 0:
        return random_string()
    elif i == 1:
        return str(random_number())
    elif i == 2:
        return random_dict()
    elif i == 3:
        return random_list()

def get_how_to_struct(result_list: list[str]) -> HowToStruct:
    struct = HowToStruct()

    for entry in result_list[1:]:
        if ":" in str(entry):
            break
        elif "var" in str(entry):
            struct.args.append(entry)
        elif "howto" in str(entry).lower():
            struct.name = entry.strip()

    return struct

def cut_invalid_line(result_list: list[str]):
    for i, entry in enumerate(result_list):
        if i == len(result_list)-1:
            break

        if ("\\n" in str(entry)) and ("\\n" in str(result_list[i+1])):
            result_list.pop(i+1)
            print("CUT INVALID LINE")
            break

def apply_how_to(result_list: list[str], env):
    struct = get_how_to_struct(result_list)
    first_entry = result_list[0]

    if "HOW TO RETURN" in first_entry:
        apply_return(result_list, env, struct)
    elif "HOW TO REPORT" in first_entry:
        apply_report(result_list, env, struct)

def apply_report(result_list: list[str], env: dict[str, list[typing.Any, int]], struct: HowToStruct):
    match len(struct.args):
            case 0:
                result_list.append(f"\\nIF {struct.name}:\\n    WRITE \"TRUE\"")
            case 1:
                var1 = random_value()
                if env[struct.args[0]][1] == 0:
                    var1 = random_string()
                elif env[struct.args[0]][1] == 1:
                    var1 = str(random_number())
                elif env[struct.args[0]][1] == 2:
                    var1 = random_dict()
                elif env[struct.args[0]][1] == 3:
                    var1 = random_list()

                result_list.append(f"\\nIF {struct.name} ({var1}):\\n   WRITE \"TRUE\"")
            case 2:
                var1 = random_value()
                if env[struct.args[0]][1] == 0:
                    var1 = random_string()
                elif env[struct.args[0]][1] == 1:
                    var1 = str(random_number())
                elif env[struct.args[0]][1] == 2:
                    var1 = random_dict()
                elif env[struct.args[0]][1] == 3:
                    var1 = random_list()

                var2 = random_value()
                if env[struct.args[1]][1] == 0:
                    var2 = random_string()
                elif env[struct.args[1]][1] == 1:
                    var2 = str(random_number())
                elif env[struct.args[1]][1] == 2:
                    var2 = random_dict()
                elif env[struct.args[1]][1] == 3:
                    var2 = random_list()

                result_list.append(f"\\nIF ({var1}) {struct.name} ({var2}):\\n  WRITE \"TRUE\"")

def apply_return(result_list: list[str], env: dict[str, list[typing.Any, int]], struct: HowToStruct):
    match len(struct.args):
            case 0:
                for k in env.keys():
                    result_list.append(f"\\n   WRITE {k}")
                if len(env.keys()) > 0:
                    result_list.append(f"\\n   RETURN {random.choice(list(env.keys()))}")
                else:
                    result_list.append(f"\\n   RETURN 42")
                result_list.append(f"\\nPUT {struct.name} IN x\\nWRITE x")
            case 1:
                for k in env.keys():
                    result_list.append(f"\\n   WRITE {k}")
                result_list.append(f"\\n   RETURN {random.choice(list(env.keys()))}")

                var1 = random_value()
                if env[struct.args[0]][1] == 0:
                    var1 = random_string()
                elif env[struct.args[0]][1] == 1:
                    var1 = str(random_number())
                elif env[struct.args[0]][1] == 2:
                    var1 = random_dict()
                elif env[struct.args[0]][1] == 3:
                    var1 = random_list()

                result_list.append(f"\\nPUT {struct.name} ({var1}) IN x\\nWRITE x")
            case 2:
                for k in env.keys():
                    result_list.append(f"\\n   WRITE {k}")
                result_list.append(f"\\n   RETURN {random.choice(list(env.keys()))}")

                var1 = random_value()
                if env[struct.args[0]][1] == 0:
                    var1 = random_string()
                elif env[struct.args[0]][1] == 1:
                    var1 = str(random_number())
                elif env[struct.args[0]][1] == 2:
                    var1 = random_dict()
                elif env[struct.args[0]][1] == 3:
                    var1 = random_list()

                var2 = random_value()
                if env[struct.args[1]][1] == 0:
                    var2 = random_string()
                elif env[struct.args[1]][1] == 1:
                    var2 = str(random_number())
                elif env[struct.args[1]][1] == 2:
                    var2 = random_dict()
                elif env[struct.args[1]][1] == 3:
                    var2 = random_list()

                result_list.append(f"\\nPUT ({var1}) {struct.name} ({var2}) IN x\\nWRITE x")

def run():
    try:
        result_list, env = generate(start, {}, 0)

        cut_invalid_line(result_list)
        apply_how_to(result_list, env)

        result = "".join(result_list).replace("\\n", "\n")
        #print(result)
        return result
    except Exception as e:
        run()

def triggersubprocess(arg: str, program: str, data_container: StdData):
    p = Popen([arg], stdout=PIPE, stdin=PIPE, stderr=PIPE, text=True)
    try:
        stdout_data, stderr_data = p.communicate(input=program, timeout=RUN_TIMEOUT)
    except Exception as e:
        p.kill()
        stdout_data, stderr_data = p.communicate()

    data_container.stderr_data = stderr_data
    data_container.stdout_data = stdout_data

# prepare clean abc binary
abc_to_test = sys.argv[1]

if os.path.basename(os.getcwd()) != "src":
    raise Exception("cwd dir MUST be src/")

script_path = os.path.relpath(os.path.dirname(os.path.realpath(__file__)))
abc_32 = script_path + "/abc_32"

if not os.path.exists(abc_32):
    raise FileNotFoundError("no abc_32 binary found! Run 'make scripts/abc_32'")

path = Path.home() / "abc"

# test loop
i = 0
failed_comp_diff = []
failed_comparison_counts = 0
success_run_counts = 0
while (i < TEST_AMOUNT):
    program = run()

    if program is None:
        continue

    d32 = StdData()
    d = StdData()

    if os.path.exists(path):
        shutil.rmtree(path, ignore_errors=True)

    t32 = Thread(
        target=triggersubprocess,
        args=[abc_32, program, d32],
    )
    t32.start()

    t = Thread(
        target=triggersubprocess,
        args=[abc_to_test, program, d],
    )
    t.start()

    t32.join(RUN_TIMEOUT)
    if t32.is_alive():
        warnings.warn(message=f"T32 thread TIMEDOUT {program}")
        continue

    t.join(RUN_TIMEOUT)
    if t.is_alive():
        warnings.warn(message=f"T thread TIMEDOUT {program}")
        continue

    if d.stderr_data != d32.stderr_data:
        failed_comparison_counts += 1
        print("Not the same error")
        failed_comp_diff.append((program, d.stdout_data, d32.stdout_data, d.stderr_data, d32.stderr_data))


    if d.stdout_data != d32.stdout_data:
        failed_comparison_counts += 1
        print("Not the same data")
        failed_comp_diff.append((program, d.stdout_data, d32.stdout_data, d.stderr_data, d32.stderr_data))

    if d32.stderr_data == "":
        success_run_counts +=1
        #print("DATA:",d32.stdout_data)
    else:
        #print(program)
        #print("ERR:",d32.stderr_data)
        ...

    i += 1
    #print(program)
    print(f"{i}/{TEST_AMOUNT}", "SUCCESS COUNT:", success_run_counts, "FAILED COMPS:", failed_comparison_counts)
    print("----------")

print("===================================================================")
for x in failed_comp_diff:
    print("========================")
    print("FAILED")
    print("Input:")
    print(x[0])
    print("64-bit branch")
    print(x[1])
    print(x[3])
    print("32-bit branch")
    print(x[2])
    print(x[4])
    print("FIX IT!!!!")
