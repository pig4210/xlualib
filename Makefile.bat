@echo off

:begin
    setlocal
    set MyPath=%~dp0

:config
    if "%1" == "" (
      set PLAT=x64
    ) else (
      set PLAT=x86
    )

    set VCPATH=C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build
    set VPATH=%MyPath%
    set GPATH=%MyPath%\\%PLAT%

    set CC=cl
    set LNK=link

    set LuaBasePath=%MyPath%..\\Lua
    set LuaExe=%LuaBasePath%\\%PLAT%\\lua

:compileflags
    set CFLAGS= /c /MP /GS- /TP /Qpar /GL /analyze- /W4 /Gy /Zc:wchar_t /Zi /Gm- /Ox /Zc:inline /fp:precise /D "WIN32" /D "NDEBUG" /D "_UNICODE" /D "UNICODE" /fp:except- /errorReport:none /GF /WX /Zc:forScope /GR- /Gd /Oy /Oi /MT /EHa /nologo /Fo"%GPATH%\\"

    set MyCFLAGS= /wd"4267" /wd"4456" /wd"4996" /I"..\\curl\\include" /I"..\\Lua\\include" /I"..\\openssl\\include" /I"..\\zlib\\include" /I"..\\xlib"

    if not "%1" == "" set MyCFLAGS=%MyCFLAGS% /D "_USING_V110_SDK71_"

:linkflags
    if "%1" == "" (
        set LFLAGS_PLAT_WINDOWS= /SUBSYSTEM:WINDOWS
    ) else (
        set LFLAGS_PLAT_WINDOWS= /SAFESEH /SUBSYSTEM:WINDOWS",5.01"
    )

    set LFLAGS= /MANIFEST:NO /LTCG /NXCOMPAT /DYNAMICBASE "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib" /MACHINE:%PLAT% /OPT:REF /INCREMENTAL:NO /OPT:ICF /ERRORREPORT:NONE /NOLOGO

    set LFLAGS=%LFLAGS% /LIBPATH:"..\\curl\\%PLAT%" /LIBPATH:"..\\Lua\\%PLAT%" /LIBPATH:"..\\openssl\\%PLAT%" /LIBPATH:"..\\zlib\\%PLAT%" /LIBPATH:"..\\xlib\\%PLAT%" /DLL

:start
    echo ==== ==== ==== ==== Prepare dest folder(%PLAT%)...

    rd /S /Q "%GPATH%" >nul
    if exist "%GPATH%" goto fail
    mkdir "%GPATH%" >nul

    echo ==== ==== ==== ==== Prepare environment(%PLAT%)...

    cd /d %VCPATH%
    if "%1" == "" (
        call vcvarsall.bat amd64 >nul
    ) else (
        call vcvarsall.bat x86 >nul
    )

    cd /d %VPATH%

:packlua
    echo ==== ==== ==== ==== Packing lua(%PLAT%)...

    "%LuaExe%" "%VPATH%\\Pack.lua" "%VPATH%\\Lua" "%VPATH%\\xlua.lua" "%VPATH%\\xlua.md" >nul
    if not %errorlevel%==0 goto fail

:res
    echo ==== ==== ==== ==== Building Resource(%PLAT%)...

    rc /D "_UNICODE" /D "UNICODE" /l 0x0409 /nologo /fo"%GPATH%\\xlualib.res" "%VPATH%\\xlualib.rc" >nul
    if not %errorlevel%==0 goto compile_error

:dll
    echo ==== ==== ==== ==== Building DLL(%PLAT%)...

    %CC% %CFLAGS% %MyCFLAGS% /Fd"%GPATH%\\xlualib.pdb" "%VPATH%\\*.cc" >nul
    if not %errorlevel%==0 goto compile_error
    
    %LNK% /OUT:"%GPATH%\\xlualib.dll" %LFLAGS% %LFLAGS_PLAT_WINDOWS% "%GPATH%\\*.obj" "%GPATH%\\xlualib.res" >nul
    if not %errorlevel%==0 goto link_error

    del "%VPATH%\\xlua.lua"
    del "%GPATH%\\*.obj"
    del "%GPATH%\\*.res"

:test
    echo ==== ==== ==== ==== Testing(%PLAT%)...

    copy "%LuaBasePath%\\%PLAT%\\luadll.exe" "%GPATH%\\lua.exe" >nul
    copy "%LuaBasePath%\\%PLAT%\\lua.dll" "%GPATH%" >nul
    cd "%GPATH%"
    lua -e "require [[xlualib]]; print( ([[AABBCC]]):show() )" >nul
    if not %errorlevel%==0 goto fail

:done
    echo.
    endlocal

    if "%1" == "" (
        cmd /C %~f0 x86
    ) else (
        exit /B 0
    )

    echo done.
    goto end

:compile_error
    echo !!!!!!!!Compile error!!!!!!!!
    goto end

:link_error
    echo !!!!!!!!Link error!!!!!!!!
    goto end

:fail
    echo !!!!!!!!Fail!!!!!!!!
    goto end

:end
    pause >nul