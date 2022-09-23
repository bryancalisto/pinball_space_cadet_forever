$slnName = 'native_library.sln'
$buildDir = 'build'
$originalPath = Get-Location

if (Test-Path $buildDir) {
  Set-Location -Path $buildDir
  msbuild $slnName
}
else {
  New-Item $buildDir -ItemType Directory
  Set-Location -Path $buildDir
  cmake ..
  msbuild $slnName
}

Set-Location -Path $originalPath