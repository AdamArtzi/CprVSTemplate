# CprVSTemplate - C++ CPR Library Visual Studio Template

 **!!! Disclosure - This project was assisted by AI. Including this README !!!**

---

This repository provides a basic Visual Studio (MSVC) C++ project template pre-configured to use the [CPR (C++ Requests) library](https://github.com/libcpr/cpr) as a static library. It includes a comprehensive test file (`Testing.cpp`) to verify the setup.

This template aims to help developers quickly start new C++ projects that require HTTP/HTTPS request capabilities without going through the manual setup process for CPR and its dependencies (libcurl, zlib) each time.

*(This template and its setup instructions were created with assistance from an AI model to ensure clarity and cover common setup steps.)*

## Features

*   **Pre-configured for CPR:** Includes necessary headers and statically linked libraries for CPR.
*   **Static Linking:** Builds CPR, libcurl, and zlib as static libraries (`.lib`) to avoid DLL deployment issues.
*   **Debug and Release Configurations:** Separate library versions for Debug and Release builds.
*   **Cross-Platform Potential:** While configured for MSVC (Windows), the principles can be adapted for other platforms/compilers.
*   **Comprehensive Test File:** `Testing.cpp` demonstrates various CPR features and helps verify the setup.
*   **Minimal Dependencies for the User:** Once cloned, the user only needs Visual Studio with C++ Desktop Development workload.

## Prerequisites

*   **Visual Studio:** 2019 or 2022 (or newer) with the "Desktop development with C++" workload installed.
*   **Git:** For cloning the repository.
*   **(Optional but Recommended for Rebuilding CPR): CMake:** If you wish to rebuild CPR or its dependencies from source yourself. This template provides pre-built libraries.

## How to Use This Template

1.  **Clone the Repository:**
    ```bash
    git clone https://github.com/YourUsername/CprVSTemplate.git CprProject
    cd CprProject
    ```
    (Replace `YourUsername` with your actual GitHub username and `CprProject` with your desired project name.)

2.  **Open in Visual Studio:**
    *   Navigate to the cloned directory (`CprProject`).
    *   Open the `.sln` (Solution) file (e.g., `CprVSTemplate.sln` or whatever you name it) with Visual Studio.

3.  **Build the Test Project:**
    *   In Visual Studio, select a configuration (e.g., "Debug") and platform (e.g., "x64").
    *   Build the solution (Build > Build Solution or F7).

4.  **Run the Test Project:**
    *   If the build is successful, run the project (Debug > Start Without Debugging or Ctrl+F5).
    *   The console window will show the output of various HTTP requests made using CPR. Check for successful status codes (mostly 200 OK) and any error messages.

5.  **Start Developing:**
    *   You can now modify `Testing.cpp` or add your own source files to start building your application using CPR.
    *   Refer to the [CPR documentation](https://docs.libcpr.org/) for API usage.

## Project Structure
```bash
CprVSTemplate/
├── External/
│ └── cpr/
│ ├── include/ # Header files for CPR, libcurl, zlib
│ │ ├── cpr/
│ │ ├── curl/
│ │ └── zlib.h, zconf.h
│ └── lib/ # Statically linked libraries
│ ├── Debug/ # Debug .lib files
│ └── Release/ # Release .lib files
├── CprVSTemplate/ # Main project folder (or your project name)
│ ├── Testing.cpp # Main test file demonstrating CPR usage
│ └── CprVSTemplate.vcxproj # Visual Studio project file
│ └── ... # Other project files
├── .gitignore
├── CprVSTemplate.sln # Visual Studio solution file
└── README.md
```

## How This Template Was Created (Tutorial for Recreating)

This section outlines the steps taken to build CPR from source and configure a Visual Studio project to use it. This is useful if you want to update CPR to a newer version or customize its build.

**Phase 1: Building CPR and its Dependencies Statically**

The goal is to build CPR, libcurl (its primary dependency), and zlib (a libcurl dependency) as static libraries (`.lib`) for both Debug and Release configurations.

1.  **Prerequisites for Building CPR:**
    *   Visual Studio (with C++ Desktop Development workload)
    *   CMake (add to PATH)
    *   Git

2.  **Download CPR Source Code:**
    ```bash
    git clone https://github.com/libcpr/cpr.git cpr_source
    cd cpr_source
    git submodule update --init --recursive # Crucial for fetching libcurl and other dependencies
    ```

3.  **Prepare a Central Installation Directory (Optional but Recommended):**
    Create a directory outside the CPR source tree where the final headers and libraries will be "installed" by CMake. This keeps things organized.
    Example: `C:/dev/third_party_libs/cpr_build_output`

4.  **Build CPR (Debug Version):**
    *   Open "x64 Native Tools Command Prompt for VS 20XX" (match your VS version).
    *   Navigate into the `cpr_source` directory.
    *   Create a build directory: `mkdir build_debug && cd build_debug`
    *   Configure CMake (adjust `CMAKE_INSTALL_PREFIX` to your chosen path):
        ```bash
        cmake .. -G "Visual Studio 17 2022" -A x64 ^
                 -DCMAKE_INSTALL_PREFIX="C:/dev/third_party_libs/cpr_build_output/debug" ^
                 -DBUILD_SHARED_LIBS=OFF ^
                 -DCPR_FORCE_USE_SYSTEM_CURL=OFF ^
                 -DCPR_BUILD_TESTS=OFF ^
                 -DCPR_BUILD_EXAMPLES=OFF ^
                 -DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreadedDebugDLL" ^
                 -DCMAKE_POLICY_DEFAULT_CMP0091=NEW
        ```
        *   `-G "Visual Studio 17 2022"`: Use the generator for your VS version (e.g., "Visual Studio 16 2019").
        *   `-A x64`: For 64-bit build.
        *   `CMAKE_INSTALL_PREFIX`: Where to copy the final `include` and `lib` folders.
        *   `BUILD_SHARED_LIBS=OFF`: Build static libraries.
        *   `CPR_FORCE_USE_SYSTEM_CURL=OFF`: Build the bundled libcurl.
        *   `CMAKE_MSVC_RUNTIME_LIBRARY="MultiThreadedDebugDLL"`: Matches `/MDd` runtime.
    *   Build and Install:
        ```bash
        cmake --build . --config Debug --target install -j %NUMBER_OF_PROCESSORS%
        ```

5.  **Build CPR (Release Version):**
    *   Navigate out of `build_debug` and back into `cpr_source`.
    *   Create a new build directory: `mkdir build_release && cd build_release`
    *   Configure CMake (note changes for Release):
        ```bash
        cmake .. -G "Visual Studio 17 2022" -A x64 ^
                 -DCMAKE_INSTALL_PREFIX="C:/dev/third_party_libs/cpr_build_output/release" ^
                 -DBUILD_SHARED_LIBS=OFF ^
                 -DCPR_FORCE_USE_SYSTEM_CURL=OFF ^
                 -DCPR_BUILD_TESTS=OFF ^
                 -DCPR_BUILD_EXAMPLES=OFF ^
                 -DCMAKE_MSVC_RUNTIME_LIBRARY="MultiThreadedDLL" ^
                 -DCMAKE_POLICY_DEFAULT_CMP0091=NEW
        ```
        *   `CMAKE_MSVC_RUNTIME_LIBRARY="MultiThreadedDLL"`: Matches `/MD` runtime.
    *   Build and Install:
        ```bash
        cmake --build . --config Release --target install -j %NUMBER_OF_PROCESSORS%
        ```

6.  **Organize Files for the Template:**
    *   After the above steps, your `C:/dev/third_party_libs/cpr_build_output/` will contain:
        ```
        debug/
        ├── include/  (cpr/, curl/, zlib.h, zconf.h)
        └── lib/      (cpr.lib, libcurl-d.lib, zlibstaticd.lib)
        release/
        ├── include/  (same as debug/include)
        └── lib/      (cpr.lib, libcurl.lib, zlibstatic.lib)
        ```
    *   Create the `External/cpr/` structure in your template project:
        ```
        YourTemplateProject/
        └── External/
            └── cpr/
                ├── include/  <-- Copy from cpr_build_output/debug/include/
                └── lib/
                    ├── Debug/    <-- Copy from cpr_build_output/debug/lib/
                    └── Release/  <-- Copy from cpr_build_output/release/lib/
        ```
    *   Ensure all necessary headers (`cpr/*`, `curl/*`, `zlib.h`, `zconf.h`) are in `External/cpr/include/`.
    *   Ensure the correct `.lib` files are in `External/cpr/lib/Debug/` and `External/cpr/lib/Release/`. (Note: libcurl debug lib is often named `libcurl-d.lib`, zlib debug `zlibstaticd.lib`).

**Phase 2: Configuring the Visual Studio Project**

1.  **Create a New C++ Project:**
    *   In Visual Studio, create a new "Console App" (C++) project. This will be your template.

2.  **Copy CPR Artifacts:**
    *   Copy the `External/cpr/` directory (created in Phase 1, Step 6) into your new Visual Studio project's root directory.

3.  **Set Project Properties (via GUI):**
    *   Right-click on your project in Solution Explorer -> Properties.

    *   **For "All Configurations" & "All Platforms" (or "x64"):**
        *   **C/C++ -> General -> Additional Include Directories:** Add `$(ProjectDir)External\cpr\include`
        *   **C/C++ -> Preprocessor -> Preprocessor Definitions:** Add `CURL_STATICLIB` (important for static libcurl linkage).
        *   **C/C++ -> Language -> C++ Language Standard:** Set to **ISO C++17 Standard (/std:c++17)** (or newer). CPR requires C++17.

    *   **For "Debug" Configuration & "x64" Platform:**
        *   **C/C++ -> Code Generation -> Runtime Library:** Set to **Multi-threaded Debug DLL (/MDd)**.
        *   **Linker -> General -> Additional Library Directories:** Add `$(ProjectDir)External\cpr\lib\debug`
        *   **Linker -> Input -> Additional Dependencies:** Add (semicolon-separated):
            `cpr.lib;libcurl-d.lib;zlibstaticd.lib;Ws2_32.lib;Wldap32.lib;Crypt32.lib;Normaliz.lib;%(AdditionalDependencies)`
            *(Adjust lib names if they differ slightly from your build, e.g., `curl-d.lib` or `zlibd.lib`)*

    *   **For "Release" Configuration & "x64" Platform:**
        *   **C/C++ -> Code Generation -> Runtime Library:** Set to **Multi-threaded DLL (/MD)**.
        *   **Linker -> General -> Additional Library Directories:** Add `$(ProjectDir)External\cpr\lib\release`
        *   **Linker -> Input -> Additional Dependencies:** Add (semicolon-separated):
            `cpr.lib;libcurl.lib;zlibstatic.lib;Ws2_32.lib;Wldap32.lib;Crypt32.lib;Normaliz.lib;%(AdditionalDependencies)`
            *(Adjust lib names if they differ slightly)*

4.  **Add Test Code:**
    *   Replace the contents of your main `.cpp` file with the code from `Testing.cpp` provided in this template.

5.  **Build and Test:**
    *   Build and run for both Debug and Release configurations to ensure everything works.

6.  **Setup `.gitignore`:**
    *   Create a `.gitignore` file. A good starting point is one generated for "VisualStudio" and "C++" (e.g., from [gitignore.io](https://www.toptal.com/developers/gitignore) or GitHub's templates).
    *   **Crucially, add exceptions** to allow your pre-built CPR libraries:
        ```gitignore
        # ... other ignores ...

        # Allow specific pre-built libraries for CPR
        !External/cpr/lib/**/*.lib
        !External/cpr/lib/**/*.pdb # Optional: if you include PDBs
        ```

7.  **Commit to Git:**
    *   Initialize a Git repository, add all files (including the `External/cpr` libs and headers), and commit.

## Troubleshooting

*   **"Cannot open include file: 'cpr/cpr.h'":** Check "Additional Include Directories."
*   **"LNK2019: unresolved external symbol":** Check "Additional Library Directories," "Additional Dependencies" (ensure all .lib files including system ones like `Ws2_32.lib` are listed), and that `CURL_STATICLIB` is defined.
*   **"LNK2038: mismatch detected for 'RuntimeLibrary'":** Ensure your project's Runtime Library setting (/MDd or /MD) matches exactly what was used to build CPR.
*   **C++17 Errors (`std::string_view`, `std::optional`, etc.):** Ensure "C++ Language Standard" is set to `/std:c++17` or newer.

## License
Feel free to use and contribute no one cares about a license.
