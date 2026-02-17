# HDCP Keygen

A Windows desktop application for generating HDCP source and sink keys, rewritten in **C++ (MFC)** based on the original Python implementation by Rich Wareham. This tool provides a graphical user interface for easy key generation and testing.

> ⚠️ **Disclaimer**  
> This is a humble attempt to recreate the original script in C++ with a simple GUI. My programming skills are limited, so it may not achieve the polished look of a professional keygen. Please use it with understanding.

## ⚠️ Important Note

This program **requires** a valid `master-key.txt` file to function. The file must be placed in the same directory as the executable. Without it, the application will fail to load and display an error message.

The format of `master-key.txt` is a 40×40 matrix of 56‑bit hexadecimal numbers.  
A reference example file (`master-key.txt`) is provided in the repository – you can use it as a template or replace it with your own master key.

## Features

- Generate random KSV (Key Selection Vector) with exactly 20 ones and 20 zeros.
- Compute source or sink private keys from the master key matrix.
- Display keys in human‑readable hexadecimal format.
- Copy KSV or key to clipboard with a single click.
- Built‑in self‑test to verify key agreement between source and sink.
- Modern Windows 10/11 visual style support.

## Requirements

- Windows 7 / 8 / 10 / 11 (x86 or x64)
- `master-key.txt` file (a sample is included in the repository)

## Building from Source

### Prerequisites

- Visual Studio 2022 or later with **Desktop development with C++** workload installed.
- MFC support (install via Visual Studio Installer).

### Steps

1. Clone the repository:
   ```bash
   git clone https://github.com/MakinoSeiran/HDCPKeygen.git
   ```
2. Open the solution file `HDCPGenerator.vcxproj` in Visual Studio.
3. Select the desired configuration (Debug/Release) and platform (x86/x64).
4. Build the solution (Ctrl+Shift+B).

The executable will be generated in the output directory (e.g., `x64\Debug\` or `Release\`).

## Usage

1. Place `master-key.txt` in the same folder as `HDCPGenerator.exe`.
2. Run the application.
3. Choose **Source** or **Sink** key type using the radio buttons.
4. Click **Generate** to create a new random KSV and corresponding key.
5. Use **Copy KSV** or **Copy Key** to copy the values to clipboard.
6. Click **Self Test** to verify the algorithm with random KSVs.

## Master Key Format

The `master-key.txt` file must contain exactly 40 rows, each with 40 hexadecimal numbers (56‑bit, up to 14 hex digits). Lines starting with `#` are ignored as comments. Example:

```
# Row 1
0123456789abcdef 123456789abcdef0 23456789abcdef01 ...
# Row 2
3456789abcdef012 456789abcdef0123 56789abcdef01234 ...
...
```

If the file is missing or malformed, the program will show an error message.

## Credits

- Original Python script by **Rich Wareham** (2010) – [GitHub](https://github.com/rjw57)
- C++/MFC port and GUI enhancements by **[MakinoSeiran](https://github.com/MakinoSeiran)**

## License

This project retains the original BSD license. See the LICENSE.txt file for details.
```
