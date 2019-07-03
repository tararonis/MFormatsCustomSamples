@setlocal enableextensions 
@cd /d "%~dp0"
 
rem copy libraries and manifests
xcopy ..\..\..\Bin\x64 .\bin\x64\Debug\DLLs /e /c /i /y /d 
copy "..\..\..\..\Medialooks Common\x64\Medialooks.Runtime.dll" .\bin\x64\Debug\DLLs\Medialooks.Runtime.dll /y 
copy ..\..\..\Bin\Manifests\x64\MServer.exe.manifest .\bin\x64\Debug\DLLs\MServer.exe.manifest /y 
copy ..\..\..\Bin\Manifests\x64\MServer.exe.manifest .\bin\x64\Debug\DLLs\DecoderLib\MServer.exe.manifest /y 
copy ..\..\..\Bin\Manifests\x64\MServer.exe.manifest .\bin\x64\Debug\DLLs\EncoderLib\MServer.exe.manifest /y 
copy ..\..\..\Bin\Manifests\x64\[YourApplicationExeFile].exe.manifest .\bin\x64\Debug\[YourApplicationExeFile].exe.manifest /y 
del ".\bin\x64\Debug\Sample File Playback.exe.manifest" /f /q 

cd .\bin\x64\Debug 
ren "[YourApplicationExeFile].exe.manifest" "Sample File Playback.exe.manifest" 

rem run sample
"Sample File Playback.exe" 

cd ..\..\..\ 
