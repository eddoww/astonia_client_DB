# GitHub Actions Workflows

This directory contains automated CI/CD workflows for the Astonia client.

## Workflows

### 1. Build (`build.yml`)

**Triggers:** Push to main/develop/claude branches, pull requests, manual dispatch

Builds the client for all three platforms:
- **Linux (Ubuntu)**: Uses apt-get to install dependencies, builds with CMake/make
- **macOS**: Uses Homebrew to install dependencies, builds with CMake/make
- **Windows**: Uses MSYS2 MinGW64 environment, builds with CMake/MinGW make

Each build:
- Compiles the client from source
- Creates platform-specific artifacts
- Uploads binaries and resources for download (30-day retention)

**Artifacts:**
- `astonia-client-linux-x64` - Linux executable + resources
- `astonia-client-macos-x64` - macOS executable + resources
- `astonia-client-windows-x64` - Windows executable + DLLs + resources

**Release Creation:**
When a tag is pushed (e.g., `v1.0.0`), the workflow automatically:
- Downloads all platform artifacts
- Creates compressed archives (tar.gz for Unix, zip for Windows)
- Creates a GitHub Release with all platform binaries attached

### 2. Build Test (`build-test.yml`)

**Triggers:** Push to test branches, manual dispatch

Matrix build strategy that tests all platforms in parallel using a single job definition.
Useful for testing build configuration changes without creating artifacts for distribution.

### 3. Code Quality (`code-quality.yml`)

**Triggers:** Push to main/develop/claude branches, pull requests

Performs static analysis and code quality checks:
- **Static Analysis**: Runs cppcheck for bug detection and portability issues
- **Code Style**: Checks for CRLF line endings, tab/space consistency, trailing whitespace
- **Build Warnings**: Compiles with extra warnings enabled (-Wall -Wextra -Wpedantic)

## Viewing Build Results

1. Go to the **Actions** tab in your GitHub repository
2. Select a workflow from the left sidebar
3. Click on a specific run to see details
4. Download artifacts from the "Artifacts" section at the bottom

## Local Testing

To test if your changes will pass CI before pushing:

### Linux
```bash
./build-linux.sh
```

### macOS
```bash
./build-macos.sh
```

### Windows (MSYS2)
```bash
mkdir build && cd build
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
mingw32-make
```

## Creating a Release

To create a new release with binaries for all platforms:

1. Ensure all changes are committed and pushed
2. Create and push a tag:
   ```bash
   git tag -a v1.0.0 -m "Release version 1.0.0"
   git push origin v1.0.0
   ```
3. GitHub Actions will automatically:
   - Build for all platforms
   - Create a GitHub Release
   - Attach platform-specific archives

## Troubleshooting

### Build Fails on Windows
- Check MSYS2 package availability
- Verify MinGW toolchain is correctly set up
- Look for DLL dependency issues in the artifact collection step

### Build Fails on macOS
- Check Homebrew formula availability
- Verify Xcode Command Line Tools are installed
- Look for code signing or permission issues

### Build Fails on Linux
- Check apt package availability (different distros use different package names)
- Verify all SDL2 development packages are installed
- Look for missing dependencies in CMake configuration

### Artifact Upload Fails
- Check if artifact size exceeds GitHub's limits
- Verify file paths are correct
- Ensure artifact directories exist before upload

## Workflow Badges

Add these to your README to show build status:

```markdown
![Build Status](https://github.com/YOUR_USERNAME/astonia_client_DB/workflows/Build%20Astonia%20Client/badge.svg)
![Code Quality](https://github.com/YOUR_USERNAME/astonia_client_DB/workflows/Code%20Quality/badge.svg)
```

Replace `YOUR_USERNAME` with your actual GitHub username.
