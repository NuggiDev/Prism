# Prism

**Universal build system detection and execution tool.**

Prism automatically detects your project type, validates required build tools, and provides a unified interface for building across multiple languages and build systems.

## Features

- 🔍 Auto-detects project type (C++, C#, Rust, Python, Node.js, Java, etc.)
- ✅ Validates required build tools before building
- 👷 Unified build interface across all languages
- 🖥️ Cross-platform support (Windows, macOS, Linux)
- 📊 Build profiling and timing
- 🛠️ Tool installation guides

## Supported Project Types

- **C++ (CMake)** - CMakeLists.txt
- **C# (.NET)** - .csproj, .sln
- **Rust (Cargo)** - Cargo.toml
- **C (Make)** - Makefile
- **JavaScript (npm)** - package.json
- **Python (pip)** - setup.py, pyproject.toml
- **Java (Maven)** - pom.xml, build.gradle

## Building from Source

### Requirements
- CMake 3.20+
- C++17 compatible compiler

### Build

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## Usage

```
prism build [path]       Detect and build project
prism rebuild [path]     Clean and rebuild
prism detect [path]      Show detected project type
prism validate [path]    Check required build tools
prism info [path]        Show project information
prism help               Show help
```

## Examples

### Build current project
```bash
prism build
```

### Build specific project
```bash
prism build /path/to/project
```

### Check if build tools are installed
```bash
prism validate
```

### Get project information
```bash
prism info
```

## License

MIT
