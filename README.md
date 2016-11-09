# KristVanity

## Usage
By running KristVanity without arguments, it will start searching for addresses on all threads without outputting to any files. The terms to look for will by default be taken from `terms.txt`.  

You can use these arguments to configure this behaviour:
* `--nonumbers` / `-n` Ignores addresses that contain numbers.
* `--log` / `-l` Determines a log file to output to. Will create a new file if needed and won't replace existing file contents.
* `--clean` / `-c` Will force KristVanity to only output address results, and this only in the format `address:password`. Useful for piping into a file.
* `--threads` / `-t` The number of threads to use for mining addresses. Will default to the amount of cores on your system.
* `--v1` / `-1` Generate Krist v1 addresses rather than v2 addresses.
* `--input` / `-i` A file containing terms to look for in addresses on each line. Defaults to `terms.txt`.
* `--help` / `-h` Displays help.

## Dependencies
KristVanity depends on [OpenSSL](https://www.openssl.org/) and [TCLAP](http://tclap.sourceforge.net/).

### Windows
Download (and build) the libraries and set their respective variables in CMake.

### Ubuntu
```
sudo apt-get install libssl-dev libtclap-dev
```

### Arch
```
sudo pacman -S openssl tclap
```
