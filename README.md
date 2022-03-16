- [Franca IDL code generation](#franca-idl-code-generation)
- [Prerequisites](#prerequisites)
  - [clone](#clone)
  - [docker](#docker)
  - [manual](#manual)
- [build:](#build)
  - [docker](#docker-1)
  - [manual](#manual-1)
- [generate documentation](#generate-documentation)
- [BUGS](#bugs)

# Franca IDL code generation

Some experiments around Franca IDL code generation
implemented in python.

Based on :
- pyfranca project  (Franca parser - does the heavy lifting)
- jinja2            (Template/generation framework)

The above projects are brought in as submodules for convenience

Plus a new addition:
- pyfranca_cpp      (jinja templates + logic to generate C++ headerfiles and classes from Franca IDL)

pyfranca_cpp is not yet separated into its own project and brought in as submodule, but might be.  It's supposed to be reusable.

# Prerequisites
## clone
```bash
git clone https://github.com/gunnarx/pyfrancagen.git
cd pyfrancagen/
git submodule update --init
```

## docker
  * Install [docker for OSD](https://inside-docupedia.bosch.com/confluence/x/5AQKJw)
  * [Docker Desktop for Windows](https://docs.docker.com/desktop/windows/install/) is **NOT allowed** to be used anymore due to new licensing models of [docker.com](https://www.docker.com/blog/the-grace-period-for-the-docker-subscription-service-agreement-ends-soon-heres-what-you-need-to-know/)

## manual
- Install clang-format or the code generation will fail
- Also read the [pyfranca_cpp/README.md](https://github.com/gunnarx/pyfrancagen/blob/master/pyfranca_cpp/README.md)

```bash
sudo apt-get install python3-pip
sudo apt-get install clang-format

pip3 install setuptools
pip3 install markupsafe==2.0.1
pip3 install ply

jinja: git checkout 2.10.x
```

# build:

First ensure submodules are brought in:
```bash
$ git submodule update --init
```
## docker
```bash
$ ./gradleDocker.sh --image="icc_generator" --run="cd icc && ./generate_icc.py"
```

## manual
```bash
$ cd icc
$ ./generate_icc.py
```
Check results in **src_gen/**

# generate documentation
Optionally, generate some Docs: (make sure Doxygen is installed)
```bash
$ cd src_gen
$ doxygen
$ firefox index.html
```
** DOCS EXAMPLE CAN BE SEEN HERE ON THE [GITHUB PAGES](https://gunnarx.github.io/pyfrancagen)

# BUGS
- Boilerplate is generated twice, (only on some file types)
- A lot more -- read the [pyfranca_cpp/README.md](https://github.com/gunnarx/pyfrancagen/blob/master/pyfranca_cpp/README.md)
