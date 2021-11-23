"""
Patches pinball binary to disable / re-enable ball decrement (infinite balls)
"""
import sys

OPERATIONS = {
    'patch': 'patch',
    'unpatch': 'unpatch'
}

# NOTE: IDA addresses have an offset of 1000C00 over the raw file addresses. For instance,
# the code located in the raw file in the address 0x00011CF2, is in 0x010128F2 in IDA.
# Remember to base the calculations on 2's complement

# SOME INSTRUCTIONS
NOP = b'\x90'
DEC = b'\x48'
CALL = b'\xE8'
RETN = b'\xC3'

# ADDRESSES
DECREMENT_BALL_ADDRESS = 0x00011CF2
END_GAME_CODE_ADDRESS = 0x00013B9B      # 0x0101479B in IDA
TOGGLE_DEMO_MODE_ADDRESS = 0x00014C02   # 0x01015802 in IDA

# CODE
CALL_SAVE_SCORE_ROUTINE = CALL + b'\x94\xEF\xff\xff' + RETN + NOP
ORIGINAL_TOGGLE_DEMO_ROUTINE = b'\x83\x3D\x70\x55\x02\x01\x00'

if len(sys.argv) != 3:
    print(f'Usage: python patchPinball.py [pathToBinary] [{" | ".join([item for item in OPERATIONS.values()])}]')
    exit(0)

filename = sys.argv[1]
op = sys.argv[2]

# FOR DEBUGGING
# with open(filename, 'r+b') as f:
#     f.seek(TOGGLE_DEMO_MODE_ADDRESS)
#     print(f.read(6).hex())
# exit(0)

try:
    with open(filename, 'r+b') as f:
        f.seek(DECREMENT_BALL_ADDRESS)

        if op == OPERATIONS['patch']:
            print('PATCHING')

            if f.read(1).hex() == NOP.hex():
                print('Aparently, this binary is already patched')
                f.close()
                exit(0)

            # Remove decrement ball instruction
            print('Writing byte {DECREMENT_BALL_ADDRESS} with NOP instruction (0x90)')
            f.seek(DECREMENT_BALL_ADDRESS)
            f.write(NOP)

            # Add functionality to save score when toggling demo
            print('Writing code to save score')
            f.seek(TOGGLE_DEMO_MODE_ADDRESS)
            f.write(CALL_SAVE_SCORE_ROUTINE)
        elif op == OPERATIONS['unpatch']:
            print('UNPATCHING')

            if f.read(1).hex() == DEC.hex():
                print('Aparently, this binary is already unpatched')
                f.close()
                exit(0)

            # Add decrement ball instruction
            print('Writing byte {DECREMENT_BALL_ADDRESS} with DEC instruction (0x48)')
            f.seek(DECREMENT_BALL_ADDRESS)
            f.write(DEC)

            # Remove functionality to save score when toggling demo
            print('Removing code to save score')
            f.seek(TOGGLE_DEMO_MODE_ADDRESS)
            f.write(ORIGINAL_TOGGLE_DEMO_ROUTINE)

    print('SUCCESS')
except Exception as e:
    print('ERROR')
    print(e)
