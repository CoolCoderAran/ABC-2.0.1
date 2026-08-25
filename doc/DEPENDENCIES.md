# DEPENDENCIES

## HOW TO GET THE DEPENDENCIES

As a first step make sure you are in the `deps/` subdirectory

```sh
cd deps/
```

### Download the dependency sources

> [!TIP]
>
> You can do this by running `./get-deps.sh`, which does the same thing,
> automatically.

Download `musl` version 1.2.6 and extract the archive

```sh
wget 'https://musl.libc.org/releases/musl-1.2.6.tar.gz'
tar -xzf musl-1.2.6.tar.gz 
```

Download ncurses version 6.6 and extract the archive

```sh
wget 'https://ftp.gnu.org/gnu/ncurses/ncurses-6.6.tar.gz'
tar -xzf ncurses-6.6.tar.gz 
```

### Build and install the dependencies

> [!TIP]
>
> You can do this by running `./build-deps.sh`, which does the same thing,
> automatically.

Set a prefix variable which we will use to build and install musl and ncurses

```sh
prefix="$PWD"
```

Build and install musl for `musl-gcc`

```sh
cd musl-1.2.6/
./configure --prefix="$prefix"
make -j$(nproc)
make install
cd ..
```

This will have installed a musl compiler under the `bin/` directory. It is
probably named something like `musl-gcc` or `musl-clang`.

You can put the built and installed binaries onto your `PATH` as shown below.
As an alternative, you can just refer to the musl compiler using an absolute
path in the following steps.

```sh
export PATH="$PATH:$prefix/bin"
```

Before you build ncurses, you need to make sure to set the possible locations of
the terminfo database. These will get compiled into the binaries as the
defaults.

The following is a best effort list compiled by Boldi. This should hopefully
work on most systems, this includes most of the places the terminfo database
might be installed by default.

```sh
export TERMINFO_DIRS="/etc/terminfo"
TERMINFO_DIRS+=":/usr/share/terminfo"
TERMINFO_DIRS+=":/lib/terminfo"
TERMINFO_DIRS+=":/usr/lib/terminfo"
TERMINFO_DIRS+=":/nix/var/nix/profiles/default/share/terminfo"
TERMINFO_DIRS+=":/run/current-system/sw/share/terminfo"
TERMINFO_DIRS+=":/nix/profile/share/terminfo"
TERMINFO_DIRS+=":/var/lib/flatpak/exports/share/terminfo"
export TERMINFO="$TERMINFO_DIRS"
```

Compile and install `ncurses` as a static library

Make sure to replace `musl-gcc` with the name to the compiler you just
installed. You can also refer to it as an absolute path if you did not add it to
your `PATH`.

```sh
cd ncurses-6.6/
./configure --with-install-prefix="$prefix" --disable-lib-suffixes CC="$musl_cc -static"
make -j$(nproc)
make install
cd ..
```

Now all that is is left is to compile ABC against our `ncurses` as a static
executable. Enjoy!

## HOW TO CLEAN DEPENDENCIES

Use Git's clean functionality to delete all ignored files under `deps/`.

```sh
git clean -xi deps/
```

Then confirm the prompt.
