# Contributing to Astonia Client

Thank you for your interest in contributing to the Astonia client!

## Development Setup

### Prerequisites

See [README_BUILD.md](../README_BUILD.md) for platform-specific setup instructions.

### Getting Started

1. Fork the repository
2. Clone your fork:
   ```bash
   git clone https://github.com/YOUR_USERNAME/astonia_client_DB.git
   cd astonia_client_DB
   ```
3. Create a feature branch:
   ```bash
   git checkout -b feature/your-feature-name
   ```

## Building

### Quick Build

Use the platform-specific build scripts:
- Linux: `./build-linux.sh`
- macOS: `./build-macos.sh`
- Windows: See README_BUILD.md

### Manual Build

```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)
```

## Continuous Integration

All pull requests are automatically built and tested on:
- Ubuntu Linux (latest)
- macOS (latest)
- Windows (via MSYS2)

You can check build status in the GitHub Actions tab.

## Code Style

### General Guidelines

- Use consistent indentation (tabs or spaces, match existing code)
- Keep lines under 120 characters when reasonable
- Add comments for non-obvious code
- Follow existing naming conventions

### Before Submitting

Run these checks locally:

```bash
# Check for common issues
cppcheck --enable=warning,performance,portability src/

# Build with warnings
mkdir build-warnings && cd build-warnings
cmake -DCMAKE_C_FLAGS="-Wall -Wextra" ..
make
```

## Platform Compatibility

### Writing Cross-Platform Code

Always use the platform abstraction layer in `src/platform.h`:

```c
#include "../../src/platform.h"

// Good - cross-platform
void* ptr = platform_heap_alloc(size);
HMODULE lib = platform_load_library("mylib");

// Bad - Windows-only
void* ptr = HeapAlloc(heap, 0, size);
HMODULE lib = LoadLibrary("mylib.dll");
```

### Platform-Specific Code

If you must write platform-specific code, use conditional compilation:

```c
#ifdef PLATFORM_WINDOWS
    // Windows-specific code
#elif defined(PLATFORM_MACOS)
    // macOS-specific code
#elif defined(PLATFORM_LINUX)
    // Linux-specific code
#endif
```

### Testing on Multiple Platforms

- Test your changes on at least one platform locally
- GitHub Actions will automatically test all platforms
- Check the Actions tab for build results

## Submitting Changes

### Pull Request Process

1. Ensure your code builds on your platform
2. Update documentation if you've changed functionality
3. Commit your changes with clear messages:
   ```bash
   git commit -m "Add feature X to improve Y"
   ```
4. Push to your fork:
   ```bash
   git push origin feature/your-feature-name
   ```
5. Create a Pull Request on GitHub
6. Wait for CI builds to complete
7. Address any review feedback

### Commit Messages

Write clear, descriptive commit messages:

```
Short summary (50 chars or less)

More detailed explanation if needed. Wrap at 72 characters.
Explain what changed and why, not just what you did.

- Bullet points are okay
- Use present tense ("Add feature" not "Added feature")
```

### Pull Request Description

Include in your PR description:
- What does this PR do?
- Why is this change needed?
- How has it been tested?
- Screenshots (if UI changes)
- Breaking changes (if any)

## Reporting Issues

### Bug Reports

Include:
- Platform (Linux/macOS/Windows)
- Build method (script/manual/CMake options)
- Steps to reproduce
- Expected vs actual behavior
- Logs or error messages
- Screenshots if applicable

### Feature Requests

Include:
- Clear description of the feature
- Why it would be useful
- How it might work
- Any implementation ideas

## Questions?

- Check existing issues and PRs first
- Read the documentation in README_BUILD.md
- Open a discussion or issue on GitHub

Thank you for contributing! 🎮
