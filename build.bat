@echo off


:: ganxi->尝试执行 python --version 命令并将错误信息重定向到临时文件
python --version 2> temp_python_version.txt
if %errorlevel% neq 0 (
    echo.
    echo xxxxxxxxxxxxxxxxxx   ERROR   xxxxxxxxxxxxxxxxxxxxxxx
    echo         Python is not installed!
    echo         Please download and install Python 3.7 
    echo xxxxxxxxxxxxxxxxxx   ERROR   xxxxxxxxxxxxxxxxxxxxxxx
    echo.
    set ret=1
    goto exit
)
del temp_python_version.txt


set project=fgm842d
set ql_tools_dir=%~dp0ql_tools
set ql_build_dir=%~dp0ql_build
set ql_output_dir=%~dp0ql_out

set ql_cmake_name=cmake-3.28.0-rc3-windows-x86_64
set ql_mingw_name=winlibs-x86_64-posix-seh-gcc-13.2.0-mingw-w64msvcrt-11.0.1-r2
set ql_gcc_name=gcc-arm-none-eabi-5_4-2016q3-20160926-win32
set cmd_7z=%ql_tools_dir%\7z\7z.exe

set build_oper=%1
if "%build_oper%"=="" (
    set oper=new
) else if /i "%build_oper%"=="r" (
    set oper=r
) else if /i "%build_oper%"=="new" (
    set oper=new
) else if /i "%build_oper%"=="n" (
    set oper=new
) else if /i "%build_oper%"=="clean" (
    set oper=clean
) else if /i "%build_oper%"=="c" (
    set oper=clean
) else if /i "%build_oper%"=="h" (
    set oper=h
) else if /i "%build_oper%"=="-h" (
    set oper=h
) else if /i "%build_oper%"=="help" (
    set oper=h
) else if /i "%build_oper%"=="/h" (
    set oper=h
) else if /i "%build_oper%"=="/?" (
    set oper=h
) else if /i "%build_oper%"=="?" (
    set oper=h
) else (
    echo=
    echo ERR: unknown build operation: %build_oper%, should be r/n/new/clean/h/-h
    echo=
    set ret=1
    goto helpinfo
)
echo=

if /i "%oper%"=="h" (
    goto helpinfo
)

if /i "%oper%"=="clean" (
    call :cleanall %CD%\ql_build\gccout %CD%\ql_out
    goto doneseg
)

if /i "%2" == "" (
    echo=
    echo We need the Project to build the firmware...
    echo=
    set ret=1
    goto helpinfo
)
set buildproj=%2

for %%i in (A B C D E F G H I J K L M N O P Q R S T U V W X Y Z) do call set buildproj=%%buildproj:%%i=%%i%%

if /i "%3" == "" (
    echo=
    echo We need the Version to build the firmware...
    echo=
    set ret=1
    goto helpinfo
)
set buildver=%3
for %%i in (A B C D E F G H I J K L M N O P Q R S T U V W X Y Z) do call set buildver=%%buildver:%%i=%%i%%
set ql_app_ver=%buildver%_APP

if not exist %ql_tools_dir%/%ql_gcc_name% ( 
    %cmd_7z% x -y %ql_tools_dir%/%ql_gcc_name%.zip -o%ql_tools_dir%/%ql_gcc_name% || ( echo ------------unzip gcc failed------------- & set ret=1 & goto exit)
    echo unzip gcc done.
)

if not exist %ql_tools_dir%/%ql_cmake_name% ( 
    %cmd_7z% x -y %ql_tools_dir%/%ql_cmake_name%.zip -o%ql_tools_dir% || ( echo ------------unzip cmake failed------------- & set ret=1 & goto exit)
    echo unzip cmake done.
)

if not exist %ql_tools_dir%/mingw64 ( 
    %cmd_7z% x -y %ql_tools_dir%/%ql_mingw_name%.7z -o%ql_tools_dir% || ( echo ------------unzip mingw failed------------- & set ret=1 & goto exit)
    echo unzip mingw done.
)

call :add_path %ql_tools_dir%/%ql_cmake_name%/bin/
call :add_path %ql_tools_dir%/mingw64/bin/
echo=
echo %PATH%

if /i "%oper%"=="new" (
    call :cleanall %CD%\ql_build\gccout %CD%\ql_out
)

if not exist "%ql_output_dir%" (
    mkdir "%ql_output_dir%"
)

cd /d "%ql_build_dir%"

if exist "gccout" (
    rmdir /s /q gccout
)
mkdir gccout

echo Entering gccout directory...
cd gccout

cmake ../.. -G Ninja
if %errorlevel% neq 0 (
    echo.
    echo xxxxxxxxxxxxxxxxxx   ERROR   xxxxxxxxxxxxxxxxxxxxxxx
    echo             CMake configuration failed!
    echo xxxxxxxxxxxxxxxxxx   ERROR   xxxxxxxxxxxxxxxxxxxxxxx
    echo.
    set ret=1
    cd ../..
    goto doneseg
)

ninja
if %errorlevel% neq 0 (
    echo.
    echo xxxxxxxxxxxxxxxxxx   ERROR   xxxxxxxxxxxxxxxxxxxxxxx
    echo             Firmware building is ERROR!
    echo xxxxxxxxxxxxxxxxxx   ERROR   xxxxxxxxxxxxxxxxxxxxxxx
    echo.
    set ret=1
    cd ../..
    goto doneseg
)

cd /d "%~dp0"

if exist "%ql_build_dir%\gccout\%project%.map" (
    copy "%ql_build_dir%\gccout\%project%.map" "%ql_output_dir%\%project%.map"
)
if exist "%ql_build_dir%\gccout\%project%.elf" (
    copy "%ql_build_dir%\gccout\%project%.elf" "%ql_output_dir%\%project%.elf"
)

@REM for /f "tokens=2 delims==" %%I in ('wmic os get localdatetime /value') do set datetime=%%I
@REM set datetime=%datetime:~0,4%-%datetime:~4,2%-%datetime:~6,2%-%datetime:~8,2%-%datetime:~10,2%-%datetime:~12,2%
for /f "usebackq delims=" %%a in (`powershell -command "Get-Date -Format 'yyyy-MM-dd-HH-mm-ss'"`) do (
    set "datetime=%%a"
)


if exist "%ql_build_dir%\gccout\%project%.bin" (
    copy "%ql_build_dir%\gccout\%project%.bin" "%ql_tools_dir%\rtt_ota\"
    cd /d "%ql_tools_dir%\rtt_ota"
    rt_ota_packaging_tool_cli.exe -f %project%.bin -v %project%%datetime% -p app -c gzip -s aes -i 0123456789ABCDEF -k 0123456789ABCDEF0123456789ABCDEF
    if %errorlevel% neq 0 (
        echo.
        echo xxxxxxxxxxxxxxxxxx   ERROR   xxxxxxxxxxxxxxxxxxxxxxx
        echo         Firmware package generation failed!
        echo xxxxxxxxxxxxxxxxxx   ERROR   xxxxxxxxxxxxxxxxxxxxxxx
        echo.
        set ret=1
        cd /d "%~dp0"
        goto doneseg
    )
    move %project%.rbl %ql_output_dir%/
) else (
    echo.
    echo xxxxxxxxxxxxxxxxxx   ERROR   xxxxxxxxxxxxxxxxxxxxxxx
    echo         Firmware binary file not found!
    echo xxxxxxxxxxxxxxxxxx   ERROR   xxxxxxxxxxxxxxxxxxxxxxx
    echo.
    set ret=1
    cd /d "%~dp0"
    goto doneseg
)

cd /d "%~dp0"

echo.
echo ********************        PASS         ***********************
echo               Firmware package is ready for you.
echo ********************        PASS         ***********************
echo.


:doneseg
echo=
echo %date%
set Start_at=%time%
echo START TIME:  %Start_at%
echo END TIME:    %time%
echo=
goto exit

:helpinfo
echo=
echo.Usage: %~n0 r/new/n Project Version [debug/release]
echo.       %~n0 clean
echo.       %~n0 h/-h
echo.Example:
echo.       %~n0 new %def_proj% your_firmware_version
echo=
echo.Supported projects include but are not limited to:
echo        FCM242D/FGM842D
echo=
echo.If you have any question, please contact Quectel.
echo=
echo=

:exit
exit /B %ret%

:add_path
(echo ";%PATH%;" | find /C /I ";%1;" > nul) || set "PATH=%1;%PATH%"
goto :eof

:cleanall
if exist "%1" (
    del /s/q/f %1 > nul
    rd /s/q %1 > nul
    echo clean "%1" done
)
if exist "%2" (
    del /s/q/f %2 > nul
    rd /s/q %2 > nul
    echo clean "%2" done
)
if exist "%3" (
    del /s/q/f %3 > nul
    rd /s/q %3 > nul
    echo clean "%3" done
)

echo=
goto :eof