# Windows MessageBox Utility

A lightweight command-line tool for displaying Windows message boxes with customizable icons, buttons, and timeout options. Perfect for batch scripts, automation, and system administration tasks.

## Features

- 🖼️ Multiple icon styles (Error, Warning, Information, Question)
- 🔘 Various button combinations (OK, OK/Cancel, Yes/No, etc.)
- ⏱️ Configurable timeout
- 🎨 Colored console output
- 💻 Windows 7/8/8.1/10/11 compatible
- 📦 Single executable, no dependencies

## Installation

### Option 1: Download Pre-built Binary
A precompiled binary (`messagebox.exe`) is available in the sources page. Simply download and use it!

### Option 2: Build from Source

#### Prerequisites
- GCC compiler (MinGW or similar)
- Windows SDK

#### Build Steps
```bash
gcc -s -Os -o messagebox.exe messagebox.c -luser32 -lkernel32
```

## Usage

```bash
messagebox.exe "message" "title" ICON_TYPE BUTTON_TYPE [timeout]
```

### Parameters
- `message`: Text to display in the message box
- `title`: Window title
- `ICON_TYPE`: One of ERROR, WARNING, INFORMATION, QUESTION
- `BUTTON_TYPE`: One of OK, OKCANCEL, RETRYCANCEL, YESNO, YESNOCANCEL
- `timeout`: (Optional) Timeout in seconds (default: 15)

### Examples

```bash
# Simple information message
messagebox.exe "Hello World!" "Greetings" INFORMATION OK

# Confirmation dialog with 10-second timeout
messagebox.exe "Delete this file?" "Confirm" QUESTION YESNOCANCEL 10

# Error message
messagebox.exe "Failed to save file!" "Error" ERROR OK

# Warning with retry option
messagebox.exe "Connection lost. Retry?" "Warning" WARNING RETRYCANCEL
```

### Return Values
The program outputs one of the following:
- OK
- Cancel
- Yes
- No
- Retry
- Timeout

## Use Cases

- Batch script user interactions
- System administration tasks
- Automated testing
- User notifications
- Confirmation dialogs

## License

The project is licensed under the terms of the MIT License.

## Author

Zig Zag

## Acknowledgments

- Windows API documentation
- Open source community
