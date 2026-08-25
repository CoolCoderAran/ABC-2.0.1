# BUILDING FROM SOURCE CODE

The build will follow the general steps of

## QUICK START

1. Install dependencies: `make`, `gcc` 14 or higher, and `ncurses`.

   Read [DEPENDENCIES](#dependencies) for details.

   On **Windows**, make sure to open Cygwin, and navigate to the location of the
   project.

2. Compile everything.

   ```sh
   make
   ```

   You can overwrite the compiler to use:

   ```sh
   make CC=gcc-15
   ```

3. Go to the `src/` folder.

   ```sh
   cd src/
   ```

4. Run ABC.

   ```sh
   ./abc
   ```

You are done. Keep reading if you want to build in different ways.

## DIFFERENT BUILDS

For details about distribution builds, see
[DISTRIBUTION.md](./doc/DISTRIBUTION.md).

If you want to compile 32-bit instead:

```sh
make BIT=32
```

> [!NOTE]
>
> If you have previously built ABC for a different architecture (eg. 32-bit
> vs 64-bit), you need to clean up the build output before trying again by
> running `make clean`

Or, if you are compiling on a system which uses a custom compiler and system
configuration (on MacOS, `gcc-15`) instead of the default one:

```sh
make CC=gcc-15
```

## DEPENDENCIES

- ncurses 6 (only termcap compatibility)
  provides termcap headers too)
- GCC 14+, or Clang 18+ (compiler needs to support C23)
- GNU make

### Install Dependencies on Ubuntu

When compiling a 64-bit executable:

```sh
sudo apt-get install build-essential libncurses-dev
```

When compiling a 32-bit executable:

```sh
sudo apt-get install build-essential gcc-multilib lib32ncurses-dev
```

### Install Dependencies on MacOS

Compilation for 64-bit platforms only.

Requires Homebrew; instructions found on the official Homebrew page:
<https://brew.sh/>

1. Install ncurses via Homebrew:

   ```sh
   brew install ncurses
   ```

2. Install GCC via Homebrew (if not already installed):

   ```sh
   brew install gcc
   ```

3. Check your GCC version by running:

   ```sh
   ls /opt/homebrew/bin/gcc*
   ```

4. Manually set the `CC` flag to the GCC version present on your computer:

   Look for a folder named `/opt/homebrew/bin/gcc-xx`, where `xx` is the current
   version number. This will tell you the GCC version you are running. When
   compiling, make sure to:

   E.g. `make CC=gcc-15`

### Dependencies for Windows

Install Cygwin from <https://www.cygwin.com/> and make sure the following
packages are installed:

- `gcc-core` (at least 14.3)
- `make`
- `libncurses-devel`

During building, make sure to execute everything from a Cygwin console.
