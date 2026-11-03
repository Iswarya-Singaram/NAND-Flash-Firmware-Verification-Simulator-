
import subprocess

# Commands sent to the C simulator
commands = """
WRITE 0 0 HELLO
READ 0 0
WRITE 0 0 AGAIN
ERASE 0
READ 0 0
WRITE 1 0 TEST
MAP 20
MAP 256
EXIT
"""

# Execute the C simulator
result = subprocess.run(
    ["nand_simulator.exe"],
    input=commands,
    capture_output=True,
    text=True
)

output = result.stdout

# Expected results
expected_read = (b"HELLO" + b"\xFF" * 11).hex().upper()
expected_erased = (b"\xFF" * 16).hex().upper()

tests = {
    "Memory initialization": "INIT PASS" in output,
    "Write operation": "WRITE PASS" in output,
    "Read operation": f"DATA {expected_read}" in output,
    "Illegal reprogramming": "Page already programmed" in output,
    "Erase operation": "ERASE PASS" in output,
    "Erase verification": f"DATA {expected_erased}" in output,
    "Bad block detection": "Bad block" in output,
    "Address mapping": "MAP PASS: Block=0 Page=1 Offset=4" in output,
    "Invalid address": "MAP FAIL: Invalid address" in output
}

# Generate test report
passed = 0

with open("test_results.log", "w") as log:

    log.write("NAND FLASH VERIFICATION REPORT\n\n")

    for name, status in tests.items():

        result_text = "PASS" if status else "FAIL"

        log.write(f"{name}: {result_text}\n")

        print(f"{name}: {result_text}")

        if status:
            passed += 1

    failed = len(tests) - passed

    log.write(f"\nTotal Tests: {len(tests)}")
    log.write(f"\nPassed: {passed}")
    log.write(f"\nFailed: {failed}")

    print("\nTotal Tests:", len(tests))
    print("Passed:", passed)
    print("Failed:", failed)

if failed:
    raise SystemExit("Some verification tests failed.")

print("\nVerification completed successfully!")

