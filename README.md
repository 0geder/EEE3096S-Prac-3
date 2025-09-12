# EEE3096S 2025 Practical 3

This repository contains three STM32 microcontroller projects for Practical 3:

- [`Practical_3A`](Practical_3A/)
- [`Practical_3F0`](Practical_3F0/)
- [`Practical_3F4`](Practical_3F4/)

## Structure

- Each project folder contains:
  - Source code (`Core/`, `Drivers/`)
  - STM32CubeMX configuration files (`*.ioc`)
  - Build files (`Makefile`, `.cproject`, `.mxproject`)
  - Startup assembly files and linker scripts

## Building

To build any project, open a terminal in the project directory and run:

```sh
make
```

This will generate the `.elf` binary in the `build/` folder.

## Programming & Debugging

- Use STM32CubeIDE or your preferred toolchain.
- The `.ioc` files can be opened in STM32CubeMX for pinout and peripheral configuration.
- Debug launch configurations are provided (e.g., `Practical_3A Default.launch`).

## Project Details

- **Practical_3A**: STM32F446RCTx (F4 series)
- **Practical_3F0**: STM32F051C6Tx (F0 series)
- **Practical_3F4**: STM32F446RCTx (F4 series)

Each project demonstrates different aspects of STM32 development, including GPIO, RCC, DMA, and more.

## Notes

- Refer to [`EEE3096S_2025_Practical_3 (5).pdf`](EEE3096S_2025_Practical_3%20(5).pdf) for instructions and requirements.
- Generated code and configuration files should not be edited manually unless necessary.
- For custom code, use the designated user code sections in the source files.

## Contact

For queries, contact your lab instructor or consult the...
