# R-Type Project Build System

This project uses CMake for building both the server and client components of the R-Type game.

## Prerequisites

### Automatic Setup (Recommended)

The project now includes autonomous setup scripts that automatically handle all dependencies:

```bash
# Linux/macOS
./setup.sh --build

# Windows (PowerShell as Administrator)
.\setup.ps1 -Build
```

### Manual Prerequisites

If you prefer manual installation:

- **CMake 3.16+**
- **C++20 compatible compiler** (GCC 10+, Clang 10+, MSVC 2019+)
- **Conan package manager** (recommended) or system packages:
  - **SFML 2.5+** (for the client)
  - **Boost.Asio** (for networking)

### Installing Dependencies Manually

#### With Conan (Cross-platform):
```bash
pip install conan
conan profile detect --force
cd build
conan install .. --build=missing
```

#### System packages (Linux only):

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install cmake g++ libsfml-dev libboost-all-dev
```

**Fedora:**
```bash
sudo dnf install cmake gcc-c++ SFML-devel boost-devel
```

**Arch Linux:**
```bash
sudo pacman -S cmake gcc sfml boost
```

## Building the Project

### Using the Build Script (Recommended)

The project includes a convenient build script:

```bash
# Build everything
./build.sh

# Build only the server
./build.sh server

# Build only the client
./build.sh client

# Clean build files
./build.sh clean

# Clean and rebuild everything
./build.sh rebuild

# Install the project
./build.sh install

# Show help
./build.sh help
```

### Manual CMake Build

Alternatively, you can use CMake directly:

```bash
# Create build directory
mkdir build
cd build

# Configure the project
cmake ..

# Build everything
make -j$(nproc)

# Or build specific targets
make rtype_server  # Server only
make rtype_client  # Client only
```

## Project Structure

```
├── CMakeLists.txt          # Root CMake file
├── build.sh                # Build script
├── Server/
│   ├── CMakeLists.txt      # Server CMake configuration
│   ├── src/                # Server source files
│   └── include/            # Server header files
├── Client/
│   ├── CMakeLists.txt      # Client CMake configuration
│   ├── src/                # Client source files
│   └── include/            # Client header files
├── All_src/                # Shared source files (SFML wrappers, scenes)
├── Include/                # Shared include files
└── Assets/                 # Game assets (sprites, sounds, fonts)
```

## Running the Applications

After building, the executables will be located in:
- Server: `./build/Server/rtype_server`
- Client: `./build/Client/rtype_client`

The Assets directory will be automatically copied to the client's build directory.

## Configuration Options

The CMake build system supports several configuration options:

- **CMAKE_BUILD_TYPE**: Debug, Release, RelWithDebInfo, MinSizeRel
- **RTYPE_USE_STANDALONE_ASIO**: Automatically set when standalone Asio is found
- **RTYPE_USE_BOOST_ASIO**: Automatically set when using Boost.Asio

Example with custom build type:
```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
```

## Troubleshooting

### SFML Not Found
If SFML is not found, make sure it's properly installed and pkg-config can find it:
```bash
pkg-config --cflags --libs sfml-all
```

### Asio/Boost Issues
The build system will try to find standalone Asio first, then fall back to Boost.Asio. Make sure at least one is installed.

### Include Path Issues
If you encounter include path issues, check that all the necessary header files are in the `Include/` directories and that the CMakeLists.txt files have the correct include paths.