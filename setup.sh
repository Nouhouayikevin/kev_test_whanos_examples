#!/bin/bash

# R-Type Autonomous Setup Script
# Automatically installs dependencies and sets up the build environment

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

print_status() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_step() {
    echo -e "${BLUE}[STEP]${NC} $1"
}

# Detect OS
detect_os() {
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        echo "linux"
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        echo "macos"
    elif [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "win32" ]]; then
        echo "windows"
    else
        echo "unknown"
    fi
}

# Check if command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Install Conan package manager
install_conan() {
    print_step "Installing Conan package manager..."
    
    if command_exists conan; then
        print_status "Conan already installed: $(conan --version)"
        return
    fi
    
    if command_exists pip3; then
        pip3 install conan
    elif command_exists pip; then
        pip install conan
    else
        print_error "pip not found. Please install Python and pip first."
        exit 1
    fi
    
    # Configure Conan profile
    conan profile detect --force
    print_status "Conan installed and configured successfully"
}

# Install CMake
install_cmake() {
    print_step "Checking CMake installation..."
    
    if command_exists cmake; then
        CMAKE_VERSION=$(cmake --version | head -n1 | cut -d" " -f3)
        print_status "CMake already installed: $CMAKE_VERSION"
        return
    fi
    
    OS=$(detect_os)
    case $OS in
        "linux")
            if command_exists apt-get; then
                sudo apt-get update && sudo apt-get install -y cmake
            elif command_exists yum; then
                sudo yum install -y cmake
            elif command_exists pacman; then
                sudo pacman -S cmake
            else
                print_error "Unknown Linux package manager. Please install CMake manually."
                exit 1
            fi
            ;;
        "macos")
            if command_exists brew; then
                brew install cmake
            else
                print_error "Homebrew not found. Please install Homebrew or CMake manually."
                exit 1
            fi
            ;;
        *)
            print_error "Unsupported OS for automatic CMake installation."
            exit 1
            ;;
    esac
}

# Install Conan dependencies
install_dependencies() {
    print_step "Installing dependencies with Conan..."
    
    if [ ! -f "conanfile.txt" ]; then
        print_error "conanfile.txt not found. Please run this script from the project root."
        exit 1
    fi
    
    # Create build directory for Conan
    mkdir -p build
    cd build
    
    # Install dependencies
    conan install .. --build=missing
    
    cd ..
    print_status "Dependencies installed successfully"
}

# Setup build environment
setup_build() {
    print_step "Setting up build environment..."
    
    mkdir -p build
    cd build
    
    # Configure with CMake
    cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
    
    cd ..
    print_status "Build environment configured successfully"
}

# Build project
build_project() {
    print_step "Building R-Type project..."
    
    cd build
    cmake --build . --config Release -j$(nproc 2>/dev/null || echo 4)
    cd ..
    
    print_status "Build completed successfully!"
    print_status "Executables located in:"
    print_status "  Server: ./build/Server/rtype_server"
    print_status "  Client: ./build/Client/rtype_client"
}

# Main setup function
main() {
    print_status "R-Type Autonomous Setup Script"
    print_status "=============================="
    
    OS=$(detect_os)
    print_status "Detected OS: $OS"
    
    # Install required tools
    install_cmake
    install_conan
    
    # Install dependencies and build
    install_dependencies
    setup_build
    
    # Build if requested
    if [ "$1" = "--build" ] || [ "$1" = "-b" ]; then
        build_project
    else
        print_status "Setup completed! To build the project, run:"
        print_status "  ./setup.sh --build"
        print_status "  or"
        print_status "  cd build && cmake --build . --config Release"
    fi
    
    print_status "Setup completed successfully! 🎉"
}

# Help function
show_help() {
    echo "R-Type Autonomous Setup Script"
    echo ""
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  -b, --build    Build the project after setup"
    echo "  -h, --help     Show this help message"
    echo ""
    echo "This script will:"
    echo "  1. Install CMake (if not present)"
    echo "  2. Install Conan package manager"
    echo "  3. Install all project dependencies via Conan"
    echo "  4. Configure the build environment"
    echo "  5. Optionally build the project"
}

# Parse command line arguments
case "${1:-}" in
    -h|--help)
        show_help
        exit 0
        ;;
    -b|--build|"")
        main "$1"
        ;;
    *)
        print_error "Unknown option: $1"
        show_help
        exit 1
        ;;
esac