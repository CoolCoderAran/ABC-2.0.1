# BUGS

## LIST OF KNOWN BUGS

* ABC statically linked binary crashes when calling two argument `min` and `max`
  functions. E.g. `"a" min "abc"` or `"b" max "abc"` causes it to segfault. This
  only happens on the MUSL statically linked binaries on Linux.

  Compiling ABC as a statically linked binary causes the min2 and max2 built-ins to break on certain parameter types as input. It breaks in the form of a segmentation fault. This happens at the type checking level, when strings are passed as parameters, in ABC. We have made serious GDB debugging efforts to find the origin of the bug that causes static linking to break these specific cases. What we have found is that the `cts` field, of a `typekind` type struct, is filled with a seemingly random hexadecimal value, while it should be 0. We have not been able to explain exactly where this could happen during the static linking phase.

* When calling `log` with a base of `1`, ABC reports a division-by-zero error.
  This is mathematically correct, since `1 log 2` = `log 2 / log 1`, but it is
  misleading.
