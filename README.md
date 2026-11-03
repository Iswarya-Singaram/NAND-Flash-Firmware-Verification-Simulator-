# NAND Flash Firmware Verification Simulator

A simplified software-based NAND Flash memory simulator developed using **C** and **Python** to demonstrate basic NAND operations and automated firmware verification.

## Features

- **Memory Initialization:** Initializes NAND memory to the erased state (`0xFF`).
- **Page Read/Write:** Simulates reading and writing data to NAND pages.
- **Block Erase:** Resets all pages within a block.
- **Bad Block Detection:** Identifies and prevents operations on defective blocks.
- **Address Mapping:** Converts logical addresses into block, page, and byte offset.
- **Automated Testing:** Uses Python to execute test commands, validate outputs, and generate test reports.

## Technologies Used

- C
- Python
- GCC
- Python `subprocess` module

## Project Structure

```text
NAND-Flash/
├── nand_simulator.c
├── test_nand.py
└── test_results.log
