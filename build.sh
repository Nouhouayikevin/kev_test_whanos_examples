#!/bin/bash

# R-Type Build Script

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check dependencies
check_dependencies() {
    print_status "Checking dependencies..."
    
    # Check for cmake
    if ! command -v cmake &> /dev/null; then
        print_error "CMake is not installed. Please run ./setup.sh to install dependencies."
        exit 1
    fi
    
    # Check if Conan dependencies are installed
    if [ -f "build/conan_toolchain.cmake" ]; then
        print_status "Conan dependencies detected"
    elif [ -f "conanfile.txt" ]; then
        print_warning "Conan dependencies not installed. Run './setup.sh' first."
    else
        print_warning "No dependency management detected. Some dependencies may be missing."
    fi
}

# Build function
build_project() {
    local target=$1
    print_status "Building $target..."
    
    # Create build directory
    mkdir -p build
    cd build
    
    # Configure (with Conan toolchain if available)
    if [ -f "conan_toolchain.cmake" ]; then
        cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake
    else
        cmake ..
    fi
    
    # Build
    if [ "$target" = "all" ]; then
        make -j$(nproc)
    elif [ "$target" = "server" ]; then
        make rtype_server -j$(nproc)
    elif [ "$target" = "client" ]; then
        make rtype_client -j$(nproc)
    else
        print_error "Unknown target: $target"
        exit 1
    fi
    
    cd ..
}

# Clean function
clean_project() {
    print_status "Cleaning build directory..."
    rm -rf build
}

# Install function
install_project() {
    print_status "Installing R-Type..."
    cd build
    make install
    cd ..
}

# Main script logic
case "$1" in
    "server")
        check_dependencies
        build_project "server"
        print_status "Server build complete! Executable: ./rtype_server"
        ;;
    "client")
        check_dependencies
        build_project "client"
        print_status "Client build complete! Executable: ./rtype_client"
        ;;
    "all"|"")
        check_dependencies
        build_project "all"
        print_status "Build complete!"
        print_status "Server: ./rtype_server"
        print_status "Client: ./rtype_client"
        ;;
    "clean")
        clean_project
        ;;
    "rebuild")
        print_status "Rebuilding project (clean + build all)..."
        clean_project
        check_dependencies
        build_project "all"
        print_status "Rebuild complete!"
        print_status "Server: ./rtype_server"
        print_status "Client: ./rtype_client"
        ;;
    "rebuild-server")
        print_status "Rebuilding server (clean + build server)..."
        clean_project
        check_dependencies
        build_project "server"
        print_status "Server rebuild complete! Executable: ./build/Server/rtype_server"
        ;;
    "rebuild-client")
        print_status "Rebuilding client (clean + build client)..."
        clean_project
        check_dependencies
        build_project "client"
        print_status "Client rebuild complete! Executable: ./build/Client/rtype_client"
        ;;
    "install")
        install_project
        ;;
    "help"|"-h"|"--help")
        echo "R-Type Build Script"
        echo "Usage: $0 [target]"
        echo ""
        echo "Targets:"
        echo "  all     - Build both server and client (default)"
        echo "  server  - Build only the server"
        echo "  client  - Build only the client"
        echo "  clean          - Clean build directory"
        echo "  rebuild        - Clean and rebuild everything"
        echo "  rebuild-server - Clean and rebuild server only"
        echo "  rebuild-client - Clean and rebuild client only"
        echo "  install        - Install the project"
        echo "  help    - Show this help message"
        ;;
    *)
        print_error "Unknown option: $1"
        echo "Use '$0 help' for usage information."
        exit 1
        ;;
esac