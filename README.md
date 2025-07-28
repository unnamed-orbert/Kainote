| Download | Help Us Translate! | Join Discord Server |
| :---: | :---: | :---: |
| [![release](https://img.shields.io/github/v/release/bjakja/Kainote.svg?maxAge=3600&label=download)](https://github.com/bjakja/Kainote/releases) | [![Translation status](https://hosted.weblate.org/widgets/kainote/-/svg-badge.svg)](https://hosted.weblate.org/engage/kainote/?utm_source=widget) | [![Discord](https://img.shields.io/discord/961361569269293077.svg?label=discord&labelColor=7289da&color=2c2f33&style=flat)](https://discord.gg/9WacFTtK6q) |

# Kainote

Kainote is a powerful subtitle editor designed for a wide range of tasks. It utilizes **FFMS2** for high-precision work like typesetting, timing, and advanced editing, and **DirectShow** for general video playback and minor subtitle adjustments.

## Features

* **Comprehensive Format Support**: Natively handles ASS, SRT, MPL2, MDVD, and TMP formats. SSA files are automatically converted to ASS upon loading.
* **Versatile Format Conversion**: Easily convert subtitles between any of the supported formats.
* **Translation Mode**: A dedicated mode that displays the original text alongside the translation field, streamlining the localization process.
* **Efficient Navigation**: Quickly seek to lines that have not yet been translated or committed to final.
* **Bulk Tagging**: Apply ASS tags to multiple selected lines simultaneously.
* **Visual Tools**: Visually adjust tags like `\pos`, `\move`, `\org`, `\clip`, `\iclip`, and vector drawings (`\p`) directly on the video frame.
* **Precision Zoom**: Zoom in on the video, even in fullscreen mode, to create highly accurate vector clips and drawings.
* **Integrated Time Shifting**: Adjust subtitle timing directly within the main grid and sync changes with the current audio/video position.
* **Advanced Audio Tools**: Visualize audio as a spectrum or waveform display. Includes an auto-splitting tool perfect for timing karaoke lyrics.
* **Automation 4 Support**: Supported Automation 4 scripts with [DependencyControl](https://github.com/TypesettingTools/DependencyControl).
* **Subtitle Comparison**: Compare two different subtitle files side-by-side in separate tabs.
* **Advanced Subtitle Filtering**: Filter the subtitle view to hide unnecessary lines and focus on your work.

## Beta Builds

You can download the latest beta version of Kainote from the link below.

[**Download Kainote Beta**](https://drive.google.com/uc?id=1ECqsrLo5d1jPoz-FKvJrS0279YeTKrmS&export=download)

**Please Note**: Beta builds are unstable and intended for testing purposes. Features may be incomplete or contain bugs. If you encounter issues or have feedback, please join our Discord server.

## Contributing

### Translations

Want to see Kainote in your native language? You can help us by contributing translations on Weblate, a user-friendly platform for localization.

[Help Translate Kainote on Weblate](https://hosted.weblate.org/engage/kainote/?utm_source=widget)

[![Translation status](https://hosted.weblate.org/widget/kainote/287x66-grey.png)](https://hosted.weblate.org/engage/kainote/)

### Support & Community

For questions, help, or to join the community, find us on Discord!

[**Join the Kainote Discord Server**](https://discord.gg/8kNAxDFgwj)

## Building from Source

### Prerequisites

Before you begin, you must install the following software:

  * **Visual Studio 2022** with the "Desktop development with C++" workload.
      * **Note**: The solution uses the x64 compiler to build the ICU library, a process that can consume up to 16GB of RAM. Ensure you have sufficient memory or a large page file configured.
  * [**DirectX SDK (June 2010)**](https://www.microsoft.com/en-us/download/details.aspx?id=6812)
  * **Windows 10/11 SDK**: This can be installed via the Visual Studio Installer.
  * **NASM**: Download and install the latest stable version of [NASM](https://www.nasm.us/). The installer must be configured to add `nasm.exe` to your system's PATH.

### 1. Initial Project Setup

1.  Download the following libraries and extract their contents directly into the `Thirdparty` folder. The final paths should look like `Thirdparty/boost`, `Thirdparty/icu`, etc.
    * [**Boost**](https://www.boost.org/releases/latest/)
    * [**icu**](https://github.com/unicode-org/icu/releases/) (Download icu4c-*-src.zip)
2.  The solution requires **FFMS2** (FFmpeg Source 2), which must be compiled from source. Follow the steps in the next section carefully.

### 2. Building Dependencies (FFmpeg & FFMS2)

These steps use **MSYS2** to create a build environment for compiling the libraries with the Microsoft Visual C++ (MSVC) compiler.

#### A. Configure MSYS2 Environment

1.  **Install MSYS2**: Download `msys2-x86_64-{date}.exe` from [msys2.org](https://www.msys2.org/) and install it to the default location, `C:/msys64`.
2.  **Enable MSVC Toolchain**: Edit the file `C:/msys64/msys2_shell.cmd`. Find the line `rem set MSYS2_PATH_TYPE=inherit` and uncomment it by removing `rem`.
3.  **Launch the Build Shell**:
      * Open a **x64 Native Tools Command Prompt for VS 2022** from your Start Menu.
      * Inside this command prompt, execute `C:/msys64/msys2_shell.cmd` to start the MSYS2 shell. All subsequent commands in this section should be run from this shell.
4.  **Install Build Tools**: Run the following commands to update the package database and install necessary tools.
    ```bash
    pacman -Syyu make diffutils pkg-config
    ```
5.  **Prevent Linker Conflict**: MSYS2 has its own `link.exe`, which conflicts with MSVC's linker. Rename it temporarily:
    ```bash
    mv /usr/bin/link.exe /usr/bin/link.exe.bak
    ```

#### B. Build FFmpeg (FFMS2 Prerequisite)

1.  **Download FFmpeg**: Download the source code for a recent, stable FFmpeg release, like [**n7.1.1**](https://github.com/FFmpeg/FFmpeg/archive/refs/tags/n7.1.1.zip). Unpack it to a simple path, e.g., `C:/ffmpeg`.
2.  **Configure & Build**: In your MSYS2 shell, navigate to the FFmpeg directory and run the following commands. The configure script prepares a 64-bit build with minimal features needed for FFMS2.
    ```bash
    # Navigate to the source directory
    cd /c/ffmpeg

    # Configure the build for MSVC
    ./configure --toolchain=msvc --enable-gpl --enable-version3 --disable-programs --disable-doc --disable-avdevice --disable-postproc --disable-avfilter --enable-dxva2 --enable-d3d11va

    # Compile and install
    make
    make install
    ```
    This will install the FFmpeg headers and libraries into `C:/msys64/usr/local`.

#### C. Build FFMS2

1.  **Download FFMS2**: Download the FFMS2 source code from its [GitHub repository](https://github.com/FFMS/ffms2/archive/refs/heads/master.zip). Unpack it to a simple path, e.g., `C:/ffms2`.
2.  **Configure & Build**: In the same MSYS2 shell, navigate to the FFMS2 directory and run its build process. It will automatically find the FFmpeg you just installed.
    ```bash
    # Navigate to the source directory
    cd /c/ffms2

    # Configure the build
    ./configure --toolchain=msvc

    # Compile and install
    make
    make install
    ```

### 3. Building Kainote

1.  **Configure Project Paths**: If you installed the Windows SDK or DirectX SDK to a location other than the `C:` drive, you must update the paths in the project properties within Visual Studio (`Project Properties > VC++ Directories`).
      * **Note**: Visual Studio can sometimes fail to find SDK paths. If you get build errors, you may need to manually add the paths in `Project Properties > C/C++ > General > Additional Include Directories` and `Linker > General > Additional Library Directories`.
2.  **Build the Solution**: After all dependencies are set up correctly, open the `Kainote.sln` solution file in Visual Studio 2022. Set the solution configuration to **Release** and the platform to **x64**, then build the project.
