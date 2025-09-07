
# 🚀 Compile and Run Your C Program

Welcome! This project includes a handy script (`run.sh`) to help you compile and run your `main.c` file with ease.

---

## 🛠️ Quick Start

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

---

## 📋 What the Script Does

- ✅ Checks if `gcc` is installed. If not, it will prompt you to install it (on macOS, use `xcode-select --install`).
- 🛠️ Compiles `main.c`.
- ▶️ Runs the program if compilation is successful.
- ❌ Shows a message if compilation fails.

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
