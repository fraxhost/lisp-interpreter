#!/bin/zsh

# Check if gcc is installed
if ! command -v gcc >/dev/null 2>&1; then
    echo "gcc is not installed. Please install it first."
    echo "On macOS, run: xcode-select --install"
    exit 1
fi

# Compile and run
if gcc main.c -o main; then
    echo "Compilation successful. Running program:"
    ./main
else
    echo "Compilation failed."
fi
