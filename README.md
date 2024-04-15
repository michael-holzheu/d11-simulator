# D11 Simulation package (Under Development)

### Contents of package

- libd11: Implementation of D11 library
  - include/lib/example.h: Prototypes
- libutil: Helper functions from s390-tools package
- examples: Example program

The package also contains the following files:

 * [LICENSE](LICENSE): The MIT license that applies to this package
 * [CHANGELOG](CHANGELOG.md): The history of turing-machine versions

### Build

```bash
~/src/repository-template (master)$ make
```

### Run example

```bash
holzheu:~/src/repository-template (master)$ examples/example  -h
Usage: example [OPTIONS]

Show an example

OPTIONS
 -i, --int-test INTEGER  Hold an integer
 -b, --bool-test         Boolean option
 -h, --help              Print this help, then exit
 -v, --version           Print version information, then exit
```
