/*
 * MessageBox Utility
 * A simple command-line tool for displaying Windows message boxes
 * Copyright (c) Dark Net Studio 2019 - 2025. All rights reserved.
 */

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Function pointer type for MessageBoxTimeoutA
typedef int (WINAPI *MessageBoxTimeoutA_t)(HWND, LPCSTR, LPCSTR, UINT, WORD, DWORD);

// Case-insensitive string comparison
int equals(const char* a, const char* b) {
    return _stricmp(a, b) == 0;
}

// Display help information with colored output
void showHelp() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("\nMessageBox Utility - Help Version 1.1\n\n");

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("Command-Line Arguments:\n");

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf("  message  : The text to display in the message box.\n");
    printf("  title    : The title of the message box window.\n");
    printf("  icon     : Icon style (ERROR, WARNING, INFORMATION, QUESTION).\n");
    printf("  buttons  : Button options (OK, OKCANCEL, RETRYCANCEL, YESNO, YESNOCANCEL).\n");
    printf("  timeout  : Timeout in seconds (optional, default: 15).\n\n");

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("Usage Examples:\n");

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf("  messagebox.exe \"Hello!\" \"Greetings\" INFORMATION OK\n");
    printf("  messagebox.exe \"Delete file?\" \"Confirm\" QUESTION YESNOCANCEL 10\n");
    printf("  messagebox.exe /?\n\n");

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("Output:\n");

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf("  Shows button pressed: OK, Cancel, Yes, No, Retry, or Timeout.\n\n");

    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("This utility is fully compatible with Windows operating systems, including Windows 7, 8, 8.1, 10, and 11, ensuring reliable performance across a wide range of environments.\n\n");

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

// Convert message box result code to string
const char* resultToString(int res) {
    switch (res) {
        case IDOK: return "OK";
        case IDCANCEL: return "Cancel";
        case IDYES: return "Yes";
        case IDNO: return "No";
        case IDRETRY: return "Retry";
        case IDTIMEOUT: return "Timeout";
        default: return "Unknown";
    }
}

// Get icon flag based on icon name
UINT getIconFlag(const char* icon) {
    if (equals(icon, "ERROR")) return MB_ICONERROR;
    if (equals(icon, "WARNING")) return MB_ICONWARNING;
    if (equals(icon, "INFORMATION")) return MB_ICONINFORMATION;
    if (equals(icon, "QUESTION")) return MB_ICONQUESTION;
    return 0;
}

// Get button flag based on button name
UINT getButtonFlag(const char* btn) {
    if (equals(btn, "OK")) return MB_OK;
    if (equals(btn, "OKCANCEL")) return MB_OKCANCEL;
    if (equals(btn, "RETRYCANCEL")) return MB_RETRYCANCEL;
    if (equals(btn, "YESNO")) return MB_YESNO;
    if (equals(btn, "YESNOCANCEL")) return MB_YESNOCANCEL;
    return 0;
}

// Main function
int main(int argc, char* argv[]) {
    // Attach to parent console or create new one
    if (!AttachConsole(ATTACH_PARENT_PROCESS)) {
        AllocConsole();
    }
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);

    // Show help if requested
    if (argc == 2 && equals(argv[1], "/?")) {
        showHelp();
        return 0;
    }

    // Validate argument count
    if (argc < 5 || argc > 6) {
        fprintf(stderr, "Invalid arguments. Use /? for help.\n");
        return 1;
    }

    // Parse command line arguments
    const char* message = argv[1];
    const char* title = argv[2];
    UINT icon = getIconFlag(argv[3]);
    UINT button = getButtonFlag(argv[4]);
    DWORD timeout = (argc == 6) ? atoi(argv[5]) * 1000 : 15000;

    // Validate icon type
    if (!icon) {
        fprintf(stderr, "Invalid icon type. Use ERROR, WARNING, INFORMATION, QUESTION.\n");
        return 1;
    }

    // Validate button type
    if (getButtonFlag(argv[4]) == 0 && !equals(argv[4], "OK")) {
        fprintf(stderr, "Invalid button type. Use OK, OKCANCEL, RETRYCANCEL, YESNO, YESNOCANCEL.\n");
        return 1;
    }   

    // Load user32.dll and get MessageBoxTimeoutA function
    UINT type = icon | button;
    HMODULE hUser32 = LoadLibraryA("user32.dll");
    if (!hUser32) {
        fprintf(stderr, "Could not load user32.dll\n");
        return 1;
    }

    MessageBoxTimeoutA_t pMessageBoxTimeoutA =
        (MessageBoxTimeoutA_t)GetProcAddress(hUser32, "MessageBoxTimeoutA");

    // Show message box with timeout if available, otherwise fall back to regular MessageBox
    int result = 0;
    if (pMessageBoxTimeoutA) {
        result = pMessageBoxTimeoutA(NULL, message, title, type, 0, timeout);
    } else {
        fprintf(stderr, "MessageBoxTimeoutA not available. Falling back to MessageBoxA (no timeout).\n");
        result = MessageBoxA(NULL, message, title, type);
    }

    // Cleanup and return result
    FreeLibrary(hUser32);
    printf("%s\n", resultToString(result));
    return 0;
}
