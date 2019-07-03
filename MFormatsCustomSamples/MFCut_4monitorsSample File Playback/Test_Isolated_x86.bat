@setlocal enableextensions 
@cd /d "%~dp0"
 
rem copy libraries and manifests
xcopy ..\..\..\Bin\x86 .\bin\x86\Debug\DLLs /e /c /i /y /d 
copy "..\..\..\..\Medialooks Common\x86\Medialooks.Runtime.dll" .\bin\x86\Debug\DLLs\Medialooks.Runtime.dll /y 
copy ..\..\..\Bin\Manifests\x86\MServer.exe.manifest .\bin\x86\Debug\DLLs\MServer.exe.manifest /y 
copy ..\..\..\Bin\Manifests\x86\MServer.exe.manifest .\bin\x86\Debug\DLLs\DecoderLib\MServer.exe.manifest /y 
copy ..\..\..\Bin\Manifests\x86\MServer.exe.manifest .\bin\x86\Debug\DLLs\EncoderLib\MServer.exe.manifest /y 
copy ..\..\..\Bin\Manifests\x86\[YourApplicationExeFile].exe.manifest .\bin\x86\Debug\[YourApplicationExeFile].exe.manifest /y 
del ".\bin\x86\Debug\Sample File Playback.exe.manifest" /f /q 

cd .\bin\x86\Debug 
ren "[YourApplicationExeFile].exe.manifest" "Sample File Playback.exe.manifest" 

rem run sample
"Sample File Playback.exe" 

cd ..\..\..\ 
