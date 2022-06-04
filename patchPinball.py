import sys
from requests import patch
from utils import getHash

OPERATIONS = {
    'patch': 'patch',
    'unpatch': 'unpatch'
}

# NOTE: IDA addresses have an offset of 1000C00 over the raw file addresses. For instance,
# the code located in the raw file in the address 0x00011CF2, is in 0x010128F2 in IDA.
# Remember to base the calculations on 2's complement

# Supported binaries hash
originalFileHash = 'faa1fde004b3c60b5a8068a7c3aab151'
patchedFileHash = '554cbf463584a1a5a525a46aee7a7912'

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

try:
    with open(filename, 'r+b') as f:
        hash = getHash(f.read())

        if op == OPERATIONS['patch']:
            if hash != originalFileHash:
                print('Cannot patch this file')
                f.close()
                exit(0)

            print('Patching...')

            # Remove decrement ball instruction
            f.seek(DECREMENT_BALL_ADDRESS)
            f.write(NOP)

            # Add functionality to save score when toggling demo
            f.seek(TOGGLE_DEMO_MODE_ADDRESS)
            f.write(CALL_SAVE_SCORE_ROUTINE)
        elif op == OPERATIONS['unpatch']:
            if hash == originalFileHash:
                print('Cannot unpatch this file')
                f.close()
                exit(0)

            print('Unpatching...')

            # Add decrement ball instruction
            f.seek(DECREMENT_BALL_ADDRESS)
            f.write(DEC)

            # Remove functionality to save score when toggling demo
            f.seek(TOGGLE_DEMO_MODE_ADDRESS)
            f.write(ORIGINAL_TOGGLE_DEMO_ROUTINE)

    print('Success! Go to play!')
except Exception as e:
    print('ERROR')
    print(e)
