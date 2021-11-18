"""
Patches pinball binary to disable / re-enable ball decrement (infinite balls)
"""
import sys

TARGET_BYTE = 0x00011CF2  # raw binary file address
NOP = b'\x90'
DEC = b'\x48'
OPERATIONS = {
    'patch': 'patch',
    'unpatch': 'unpatch'
}

if len(sys.argv) != 3:
    print(f'Usage: python patchPinball.py [pathToBinary] [{" | ".join([item for item in OPERATIONS.values()])}]')
    exit(0)

filename = sys.argv[1]
op = sys.argv[2]

try:
    with open(filename, 'r+b') as f:
        f.seek(TARGET_BYTE)

        if op == OPERATIONS['patch']:
            print('PATCHING')

            if f.read(1).hex() == NOP.hex():
                print('Aparently, this binary is already patched')
                f.close()
                exit(0)

            print('Writing byte {TARGET_BYTE} with NOP instruction (0x90)')
            f.seek(TARGET_BYTE)
            f.write(NOP)
        elif op == OPERATIONS['unpatch']:
            print('UNPATCHING')

            if f.read(1).hex() == DEC.hex():
                print('Aparently, this binary is already unpatched')
                f.close()
                exit(0)

            print('Writing byte {TARGET_BYTE} with DEC instruction (0x48)')
            f.seek(TARGET_BYTE)
            f.write(DEC)

    print('SUCCESS')
except Exception as e:
    print('ERROR')
    print(e)
