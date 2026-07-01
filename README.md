# HexOS_! — Built hex by hex

<p align="center">
  <img src="logo.png" alt="HexOS_! Logo" width="230" />
</p>

<p align="center">
  <strong>A minimal, custom, and purely educational operating system built entirely from scratch using Linux From Scratch (LFS) sources.</strong><br />
  No automatic installers, no pre-compiled package managers, no shortcuts. Every single block of code, library, and kernel module has been manually configured, linked, and compiled.
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Architecture-x86__64-5B85C9?style=for-the-badge&logo=linux" alt="Arch">
  <img src="https://img.shields.io/badge/Base-Linux%20From%20Scratch-3DCB9E?style=for-the-badge" alt="LFS">
  <img src="https://img.shields.io/badge/Status-Development%20%5Bv0.6%5D-E8A23D?style=for-the-badge" alt="Status">
</p>

---

## Project Vision.

**HexOS_!** was born from the desire to understand exactly what happens under the hood of a Linux distribution. Instead of relying on pre-packaged distributions (like Ubuntu, Arch, or Debian), we chose to embrace the challenge of **LFS (Linux From Scratch)**: creating a complete software ecosystem starting directly from raw source code.

The ultimate goal is to build a cutting-edge, extremely lightweight, secure, and bloat-free operating system, where every single executable file on the disk exists for a specific reason and has been optimized for our hardware.

---

## Meet the Core Team.

The operating system is being developed by the two of us, leveraging a strategic split of roles to cover the entire engineering pipeline:

* **MRX (Kernel Architect & Toolchain Engineer)**
  * *Focus:* Managing cross-compilation, isolating the host environment, optimizing the Linux Kernel `.config` file, and configuring native hardware modules. The guardian of low-level performance and stability.
* **HexFud (SysOps Developer & User-Space Engineer)**
  * *Focus:* Managing user environments, writing initialization scripts (*System V Init*), configuring network daemons, shadow file security, and structuring the system directory tree.

---

## How We Are Creating It (Roadmap & Architecture)

The development of HexOS_! follows a rigorous process divided into critical stages, each requiring lengthy and meticulous compilation phases:

### 1. Host Environment Preparation & Partitioning:
The journey begins on a clean host system. We isolated a dedicated portion of the hard drive for HexOS_!:
* Creating block tables via `fdisk`.
* Formatting the main partition as `ext4` and the boot partition as `vfat` (UEFI).
* Mounting the target points to `/mnt/hexos`.

### 2. Compiling the Temporary Toolchain (The "Bootstrap"):
To prevent the host system from contaminating our new OS, we compiled an isolated toolchain (fundamental compiler, linker, and utilities) that temporarily resides in a separate directory:
* **Binutils:** For object file manipulation.
* **GCC (GNU Compiler Collection):** The core compiler, configured to target our specific architecture.
* **Glibc:** The essential standard C runtime libraries needed for future programs to talk to the Kernel.

### 3. Entering the Chroot Environment & Cutting the Cord:
Once the toolchain was ready, we severed ties with the host system. Using `chroot` (change root) commands, we virtually moved inside our new partition, mounting the kernel's virtual file systems (`/proc`, `/sys`, `/dev`). From this point onward, HexOS_! compiles itself using only its own tools.

### 4. Building the Base System (User-Space):
In this phase, we compile all the software and utilities that make a Linux system usable from the terminal (e.g., `Bash`, `Coreutils`, `Diffutils`, `Findutils`, `Grep`, `Sed`, `Tar`, etc.). Every package is configured, compiled with `make`, tested, and installed.

### 5. Configuring & Compiling the Linux Kernel (The Heart):
No generic kernels with thousands of unneeded drivers. We pulled the official stable Linux source code (6.x branch) and customized the `.config` file by hand, leaving only the hardware support strictly necessary for our machines. The result is an incredibly lightweight binary (`vmlinuz`) with lightning-fast boot times.

### 6. Bootloader & Initialization Scripts:
To make the system independently bootable:
* Installing and writing the **GRUB** configuration within `/boot/grub/grub.cfg`.
* Manually writing runtime scripts to handle *Runlevels* (starting services, checking file systems, mounting disks).
* Configuring network files, the host name (`/etc/hostname`), and encrypting user account credentials.

---

## Current Project Status.

HexOS_! is currently at version **v0.6**. 
The toolchain is stable, the chroot environment is properly configured, and the first test compilations of our custom kernel have been successful. The next steps include refining automation scripts for network services and creating our custom login environment.