# Dealing with git submodules

## Add a submodule
git submodule add <url> <path>

## Update a submodule
```bash
    git submodule update --init --recursive
```

## Remove a submodule
git submodule deinit -f -- a/submodule
rm -rf .git/modules/a/submodule
git rm -f a/submodule

## Remove a submodule folder
git rm --cached path_to_submodule (no trailing slash)
rm -rf .git/modules/path_to_submodule

## Clone a repo with submodules
git clone --recursive <url>

## reload missing submodules
git submodule update
