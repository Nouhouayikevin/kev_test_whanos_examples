# R-Type Autonomous Setup Script for Windows
# PowerShell script for automatic dependency management and build setup

param(
    [switch]$Build,
    [switch]$Help
)

# Color functions for output
function Write-Status {
    param($Message)
    Write-Host "[INFO] $Message" -ForegroundColor Green
}

function Write-Warning {
    param($Message)
    Write-Host "[WARNING] $Message" -ForegroundColor Yellow
}

function Write-Error {
    param($Message)
    Write-Host "[ERROR] $Message" -ForegroundColor Red
}

function Write-Step {
    param($Message)
    Write-Host "[STEP] $Message" -ForegroundColor Blue
}

# Check if command exists
function Test-Command {
    param($CommandName)
    return [bool](Get-Command -Name $CommandName -ErrorAction SilentlyContinue)
}

# Install Chocolatey (Windows package manager)
function Install-Chocolatey {
    Write-Step "Checking Chocolatey installation..."
    
    if (Test-Command choco) {
        Write-Status "Chocolatey already installed"
        return
    }
    
    Write-Step "Installing Chocolatey..."
    Set-ExecutionPolicy Bypass -Scope Process -Force
    [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072
    Invoke-Expression ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))
    
    Write-Status "Chocolatey installed successfully"
}

# Install Python and pip
function Install-Python {
    Write-Step "Checking Python installation..."
    
    if (Test-Command python) {
        Write-Status "Python already installed: $(python --version)"
        return
    }
    
    Write-Step "Installing Python via Chocolatey..."
    choco install python -y
    refreshenv
    
    Write-Status "Python installed successfully"
}

# Install CMake
function Install-CMake {
    Write-Step "Checking CMake installation..."
    
    if (Test-Command cmake) {
        $cmakeVersion = (cmake --version).Split("`n")[0]
        Write-Status "CMake already installed: $cmakeVersion"
        return
    }
    
    Write-Step "Installing CMake via Chocolatey..."
    choco install cmake -y
    refreshenv
    
    Write-Status "CMake installed successfully"
}

# Install Conan
function Install-Conan {
    Write-Step "Installing Conan package manager..."
    
    if (Test-Command conan) {
        Write-Status "Conan already installed: $(conan --version)"
        return
    }
    
    Write-Step "Installing Conan via pip..."
    python -m pip install conan
    
    # Configure Conan profile
    conan profile detect --force
    Write-Status "Conan installed and configured successfully"
}

# Install Visual Studio Build Tools
function Install-BuildTools {
    Write-Step "Checking Visual Studio Build Tools..."
    
    $vsWhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
    if (Test-Path $vsWhere) {
        $vs = & $vsWhere -latest -products * -requires Microsoft.VisualStudio.Workload.VCTools -format json | ConvertFrom-Json
        if ($vs) {
            Write-Status "Visual Studio Build Tools already installed"
            return
        }
    }
    
    Write-Step "Installing Visual Studio Build Tools via Chocolatey..."
    choco install visualstudio2022buildtools -y
    choco install visualstudio2022-workload-vctools -y
    
    Write-Status "Visual Studio Build Tools installed successfully"
}

# Install dependencies via Conan
function Install-Dependencies {
    Write-Step "Installing dependencies with Conan..."
    
    if (-not (Test-Path "conanfile.txt")) {
        Write-Error "conanfile.txt not found. Please run this script from the project root."
        exit 1
    }
    
    # Create build directory for Conan
    if (-not (Test-Path "build")) {
        New-Item -ItemType Directory -Path "build" | Out-Null
    }
    
    Set-Location "build"
    
    # Install dependencies
    conan install .. --build=missing
    
    Set-Location ".."
    Write-Status "Dependencies installed successfully"
}

# Setup build environment
function Setup-Build {
    Write-Step "Setting up build environment..."
    
    if (-not (Test-Path "build")) {
        New-Item -ItemType Directory -Path "build" | Out-Null
    }
    
    Set-Location "build"
    
    # Configure with CMake
    cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
    
    Set-Location ".."
    Write-Status "Build environment configured successfully"
}

# Build project
function Build-Project {
    Write-Step "Building R-Type project..."
    
    Set-Location "build"
    cmake --build . --config Release --parallel
    Set-Location ".."
    
    Write-Status "Build completed successfully!"
    Write-Status "Executables located in:"
    Write-Status "  Server: .\build\Server\Release\rtype_server.exe"
    Write-Status "  Client: .\build\Client\Release\rtype_client.exe"
}

# Show help
function Show-Help {
    Write-Host "R-Type Autonomous Setup Script for Windows"
    Write-Host ""
    Write-Host "Usage: .\setup.ps1 [OPTIONS]"
    Write-Host ""
    Write-Host "Options:"
    Write-Host "  -Build     Build the project after setup"
    Write-Host "  -Help      Show this help message"
    Write-Host ""
    Write-Host "This script will:"
    Write-Host "  1. Install Chocolatey package manager"
    Write-Host "  2. Install Python, CMake, and Visual Studio Build Tools"
    Write-Host "  3. Install Conan package manager"
    Write-Host "  4. Install all project dependencies via Conan"
    Write-Host "  5. Configure the build environment"
    Write-Host "  6. Optionally build the project"
}

# Main function
function Main {
    if ($Help) {
        Show-Help
        return
    }
    
    Write-Status "R-Type Autonomous Setup Script for Windows"
    Write-Status "=========================================="
    
    # Check if running as Administrator
    $currentPrincipal = New-Object Security.Principal.WindowsPrincipal([Security.Principal.WindowsIdentity]::GetCurrent())
    if (-not $currentPrincipal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) {
        Write-Warning "This script should be run as Administrator for best results."
        Write-Warning "Some installations may fail without administrator privileges."
    }
    
    try {
        # Install required tools
        Install-Chocolatey
        Install-Python
        Install-CMake
        Install-BuildTools
        Install-Conan
        
        # Install dependencies and setup build
        Install-Dependencies
        Setup-Build
        
        # Build if requested
        if ($Build) {
            Build-Project
        }
        else {
            Write-Status "Setup completed! To build the project, run:"
            Write-Status "  .\setup.ps1 -Build"
            Write-Status "  or"
            Write-Status "  cd build; cmake --build . --config Release"
        }
        
        Write-Status "Setup completed successfully! 🎉"
    }
    catch {
        Write-Error "An error occurred during setup: $_"
        exit 1
    }
}

# Run main function
Main