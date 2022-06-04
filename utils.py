import hashlib


def getHash(binaryData):
    return hashlib.md5(binaryData).digest().hex()
