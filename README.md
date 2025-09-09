
# 🚀 Lisp Interpreter Project

Welcome! This project is a simple Lisp interpreter written in C.

---

## 📂 Project Structure

```
lisp-interpreter/
|-- main.c           # Main source file (entry point)
|-- main             # Compiled executable (ignored by git)
|-- run.sh           # Script to compile and run main.c
|-- runTests.sh      # Script to run tests
|-- resources/       # Test resources and data
|   |-- sprint1/
|   |   |-- input/
|   |   |-- output/
|   |   |-- expected/
|   |   |-- test_description.txt
|   |   |-- test_report.txt
|   |-- sprint2/
|-- .gitignore       # Git ignore file
|-- README.md        # Project documentation
```

---

## 🏗️ Build Process

You can build and run the project using the provided script:

1. **Open a terminal** and navigate to your project directory:

   ```sh
   cd path/to/your/project
   ```

2. **Make the script executable** (only needed once):

   ```sh
   chmod +x run.sh
   ```

3. **Run the script**:

   ```sh
   ./run.sh
   ```

This will:

- Check if `gcc` is installed. If not, it will prompt you to install it (on macOS, use `xcode-select --install` or `brew install gcc`).
- Compile `main.c`.
- Run the program if compilation is successful.
- Show a message if compilation fails.

---

## 🗂️ Code Organization

- The main entry point is `main.c`.
- All test resources are under the `resources/` directory, organized by sprint and type (input, output, expected, etc).
- Scripts for building and testing are provided (`run.sh`, `runTests.sh`).
- The compiled executable is named `main` and is ignored by git (see `.gitignore`).

---

## 🧩 Troubleshooting

- ❗ If you see "Compilation failed.", check your `main.c` for errors.
- 🖥️ If you see "gcc is not installed", install it:
  - On macOS, use:

    ```sh
    xcode-select --install
    ```

  - Or with Homebrew:

    ```sh
    brew install gcc
    ```

---

Enjoy coding! 🎉
