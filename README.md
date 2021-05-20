# ParaCL

![GitHub repo size](https://img.shields.io/github/repo-size/derzhavin3016/ParaCL?style=for-the-badge)
![GitHub last commit](https://img.shields.io/github/last-commit/derzhavin3016/ParaCL?color=red&style=for-the-badge)
![GitHub lines](https://img.shields.io/tokei/lines/github/derzhavin3016/ParaCL?style=for-the-badge)
|Project|Interpreter | Compiler |
|:--:|:-----------:| :-: |
|Current level|  2 | 2|

Programming C-like language for iLab 2nd course.

by [**Derzhavin Andrey**](https://github.com/derzhavin3016) && [**Khaidari Farid**](https://github.com/Tako-San) && [**Shurygin Anton**](https://github.com/uslsteen).


## Install
### Requirements
- Cmake
- flex
- bison (3.7 or higher)
### Installation
After cloning, in root directory of a repo:
```bash
mkdir build
cd build
cmake ..
make # -j (optional)
cd ..
```
After that, in directory you will have two executables: `ParaCL` - LLVMIR compiler,
`ParaCLi` - interpreter.

Examples are located in directory `test` - files `.cl`.

## Usage
### Interpreter
Just run in root directory of a project:
```bash
./build/ParaCLi PARACL_FIILENAME
```
### Compiler
Just run in root directory of a project:
```bash
./build/ParaCL PARACL_FIILENAME
```
You will get a file `PARACL_FIILENAME.ll`.
After that, for compiling you need
ParaCL standard library which is located in root directory of a project - file `pcl_std.cc`.
To compile it, do
```bash
clang++ pcl_std.cc ll_file
```
You will get an executable file `a.out` in your current directory. To run it:
```bash
./a.out
```

## How it works

* Parsing by flex and bison
* Building AST
* Executing or codegeneration of LLVM IR
