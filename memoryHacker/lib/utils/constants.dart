class Msg {
  static String wrongHash = "This file isn't supported. Make sure the PE md5 hash is faa1fde004b3c60b5a8068a7c3aab151";
  static String noWindow = "The application isn't running";
  static String noOpenProcess = noWindow;
  static String readMemory = "Couldn't process the program's memory";
  static String writeMemory = readMemory;
  static String unexpectedBytecode = wrongHash;
}

class Codes {
  static int noWindow = -1;
  static int noOpenProcess = -2;
  static int readMemory = -3;
  static int writeMemory = -4;
  static int noOperation = -5;
  static int unexpectedBytecode = -6;
  static int ok = 0;
}

final pinballAppIsNotPresentCodes = [Codes.noWindow, Codes.noOpenProcess];

final msgMap = {
  Codes.noWindow: Msg.noWindow,
  Codes.noOpenProcess: Msg.noOpenProcess,
  Codes.readMemory: Msg.readMemory,
  Codes.writeMemory: Msg.writeMemory,
  Codes.unexpectedBytecode: Msg.unexpectedBytecode
};
