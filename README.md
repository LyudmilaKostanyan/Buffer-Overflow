# Buffer Overflow Experiment

## Overview

This project demonstrates a deliberate stack buffer overflow in a small C++ program and shows how various compiler protections respond to it. It highlights how memory layout vulnerabilities arise in unmanaged languages and how tools like `-fstack-protector` and `AddressSanitizer` can detect them.

## Problem Description

A local array on the stack is allocated with a small size. A larger string is copied into it without bounds checking, causing an overflow. This overflow may overwrite nearby variables or control data such as return addresses. Depending on the compilation flags used, the program may crash silently, abort with a diagnostic, or display a detailed report of the memory violation.
### Why does it crash?

* **Without protection**: the overflow may go unnoticed, but may still overwrite return addresses or local variables.
* **With `-fstack-protector-all`**: a canary is placed on the stack and detected upon corruption, causing an abort.
* **With `-fsanitize=address`**: redzones and runtime checking allow precise detection and reporting of overflow.

## Example Output

### Without Protection (Ubuntu/macOS):

```
Before overflow: adjacentVar = 1234
After overflow: adjacentVar = 1234
Aborted (core dumped)
```

### With `-fstack-protector-all`:

```
Before overflow: adjacentVar = 1234
*** stack smashing detected ***: terminated
Aborted (core dumped)
```

### With `-fsanitize=address`:

```
Before overflow: adjacentVar = 1234
==2236==ERROR: AddressSanitizer: stack-buffer-overflow on address 0x...
WRITE of size 25 at 0x... thread T0
    #0 0x... in wrap_strcpy ...
    #1 0x... in vulnerableFunction() main.cpp:10
    #2 0x... in main main.cpp:16
SUMMARY: AddressSanitizer: stack-buffer-overflow
```

## Explanation of Output

* In all cases, the 25-byte string overflows the 8-byte buffer.
* `AddressSanitizer` shows the exact memory violation, including the offending address and memory region.
* The stack protector terminates execution upon detecting a canary overwrite.
* The unprotected version may silently corrupt memory or crash unexpectedly.

## How to Compile and Run

### 1. Clone the Repository

```bash
git clone https://github.com/LyudmilaKostanyan/Buffer-Overflow.git
cd Buffer-Overflow
```

### 2. Build the Project

#### a. Without Protection

```bash
g++ -o overflow main.cpp
```

#### b. With Stack Protector

```bash
g++ -fstack-protector-all -o overflow_protected main.cpp
```

#### c. With AddressSanitizer (Linux or macOS only)

```bash
# macOS
clang++ -fsanitize=address -g -O1 -o overflow_asan main.cpp

# Linux
g++ -fsanitize=address -g -O1 -o overflow_asan main.cpp
```

> **Note:** On Windows with MinGW, AddressSanitizer is not supported.

### 3. Run the Program

```bash
# Run without protection
./overflow

# Run with stack protector
./overflow_protected

# Run with AddressSanitizer
./overflow_asan
```
