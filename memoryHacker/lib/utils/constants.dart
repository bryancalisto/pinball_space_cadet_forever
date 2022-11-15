class Msg {
  static String wrongHash = "This file isn't supported. Make sure the PE md5 hash is faa1fde004b3c60b5a8068a7c3aab151";
  static String noWindow = "The application isn't running";
  static String noOpenProcess = noWindow;
  static String readMemory = "Couldn't process the program's memory";
  static String writeMemory = readMemory;
  static String unexpectedBytecode = wrongHash;
}

final msgMap = {
  -1: Msg.noWindow,
  -2: Msg.noOpenProcess,
  -3: Msg.readMemory,
  -4: Msg.writeMemory,
  -6: Msg.unexpectedBytecode
};
