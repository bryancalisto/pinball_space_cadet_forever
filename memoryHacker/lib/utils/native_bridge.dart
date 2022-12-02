import 'dart:ffi' as ffi;
import 'dart:io' show Directory;

import 'package:path/path.dart' as path;
import 'package:ffi/ffi.dart';

typedef GetExeFilePath = ffi.Pointer<Utf16> Function();

typedef CFreeWChar = ffi.Void Function(ffi.Pointer<Utf16>);
typedef FreeWChar = void Function(ffi.Pointer<Utf16>);

typedef CToggleHackFunc = ffi.Int Function(ffi.Bool);
typedef ToggleHack = int Function(bool);

typedef CIsHackActiveFunc = ffi.Int Function();
typedef IsHackActive = int Function();

final libraryPath = path.join(Directory.current.path, 'native.dll');

final dylib = ffi.DynamicLibrary.open(libraryPath);

// Dart functions
final GetExeFilePath nativeGetExeFilePath =
    dylib.lookup<ffi.NativeFunction<GetExeFilePath>>('getExeFilePath').asFunction();

final FreeWChar nativeFreeWChar = dylib.lookup<ffi.NativeFunction<CFreeWChar>>('freeWChar').asFunction();

final ToggleHack nativeToggleHack = dylib.lookup<ffi.NativeFunction<CToggleHackFunc>>('toggleHack').asFunction();

final IsHackActive nativeIsHackActive =
    dylib.lookup<ffi.NativeFunction<CIsHackActiveFunc>>('isHackActive').asFunction();
