import 'dart:ffi' as ffi;
import 'dart:io' show Directory;

import 'package:path/path.dart' as path;

typedef CToggleHackFunc = ffi.Int Function(ffi.Bool);
typedef ToggleHack = int Function(bool);

typedef CIsHackActiveFunc = ffi.Int Function();
typedef IsHackActive = int Function();

final libraryPath = path.join(Directory.current.path, 'native_library\\build', 'Debug', 'native.dll');

final dylib = ffi.DynamicLibrary.open(libraryPath);

final ToggleHack nativeToggleHack = dylib.lookup<ffi.NativeFunction<CToggleHackFunc>>('toggleHack').asFunction();
final IsHackActive nativeIsHackActive =
    dylib.lookup<ffi.NativeFunction<CIsHackActiveFunc>>('isHackActive').asFunction();
