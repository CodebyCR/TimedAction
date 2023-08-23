# CMake

## CMake in a nutshell
- ...

### First run this one time
```bash
  export GTEST_COLOR=1
```

### Build 

```bash
    mkdir ../build
    cd ../build
    cmake ..
```

### Run

```bash
    mkdir ../build
    cd ../build
    cmake --build .
    ./TimedAction
```

### Test

```bash
    mkdir ../build
    cd ../build
    cmake --build .
    ctest --extra-verbose
```