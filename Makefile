# Makefile to configure the toolchain for the project
# Separate build commands for Windows and Linux

# Compiler flags
CFLAGS = -m64
CXXFLAGS = -m64
ASMFLAGS = -m64

# Set explicit paths for Windows compilers
WIN_C_COMPILER := C:/cygwin64/bin/gcc.exe
WIN_CXX_COMPILER := C:/cygwin64/bin/g++.exe
WIN_ASM_COMPILER := C:/cygwin64/bin/as.exe

# win: Prepare build for Windows
# linux: Prepare build for Linux

# wingui: Build for Windows (optimization O2)
# winconsole: Build for Windows (optimization O2) with console window
# golink: Build for Windows with GoLink linker (optimization O2)

# Use standard Linux paths for compilers
C_COMPILER := $(shell which gcc)
CXX_COMPILER := $(shell which g++)
ASM_COMPILER := $(shell which as)

# Build for Windows
win:
	@echo "Building for Windows"
	@if [ -f "$(WIN_C_COMPILER)" ]; then \
		echo "GCC C compiler found at $(WIN_C_COMPILER)"; \
	else \
		echo "GCC C compiler not found at $(WIN_C_COMPILER)"; \
		exit 1; \
	fi
	@if [ -f "$(WIN_CXX_COMPILER)" ]; then \
		echo "G++ C++ compiler found at $(WIN_CXX_COMPILER)"; \
	else \
		echo "G++ C++ compiler not found at $(WIN_CXX_COMPILER)"; \
		exit 1; \
	fi
	@if [ -f "$(WIN_ASM_COMPILER)" ]; then \
		echo "GNU Assembler (GAS) found at $(WIN_ASM_COMPILER)"; \
	else \
		echo "GNU Assembler (GAS) not found at $(WIN_ASM_COMPILER)"; \
		exit 1; \
	fi
	@echo "Cleaning and setting up build directory..."
	rm -rf build
	mkdir build

# Build for Linux
linux:
	@echo "Building for Linux"
	@if [ -n "$(C_COMPILER)" ]; then \
		echo "GCC C compiler found at $(C_COMPILER)"; \
	else \
		echo "GCC C compiler not found on Linux."; \
		exit 1; \
	fi
	@if [ -n "$(CXX_COMPILER)" ]; then \
		echo "G++ C++ compiler found at $(CXX_COMPILER)"; \
	else \
		echo "G++ C++ compiler not found on Linux."; \
		exit 1; \
	fi
	@if [ -n "$(ASM_COMPILER)" ]; then \
		echo "GNU Assembler (GAS) found at $(ASM_COMPILER)"; \
	else \
		echo "GNU Assembler (GAS) not found on Linux."; \
		exit 1; \
	fi
	@echo "Cleaning and setting up build directory..."
	rm -rf build
	mkdir build

wingui:
	cd build && gcc -D_WIN32 -DNDEBUG -O3 -s -o window ../tests/test_window.c -mwindows
	cd build && strip --strip-unneeded window

winconsole:
	cd build && gcc -D_WIN32 -O3 -s -o window ../tests/test_window.c
	cd build && strip --strip-unneeded window

golink:
	cd build && gcc -D_GOLINK -D_WIN32 -DNDEBUG -O3 -c -o window.obj ../tests/test_window.c
	cd build && GoLink /entry WinMain window.obj user32.dll kernel32.dll msvcrt.dll

hash:
	cd build && gcc -D_WIN32 -DNDEBUG -O3 -s -o window ../tests/perfect_hash.c

new_hash:
	cd build && gcc -D_WIN32 -DNDEBUG -O3 -s -o window ../tests/new_hash.c