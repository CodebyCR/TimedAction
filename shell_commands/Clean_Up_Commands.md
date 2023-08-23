# Clean up

## Clean CMake

```bash
    rm -rf ../build
```

```bash
    # Only for CLion CMake generated folders
    rm -rf ../cmake-build-debug
    rm -rf ../cmake-build-release
    rm -rf ../cmake-build-release-event-trace
```

## Clean Bazel

```bash
    bazel clean
```