echo off

rem �������ڱ���ͼ��ܵ�Lua����·��
set LuaBasePath=..\Lua

rem �ϲ��ļ��ű�����Ϊxlua.lua��ע�ͱ���Ϊxlua.txt
set DestLua=xlua.lua
set DestCmt=xlua.txt

rem ��ת����BAT���ڵ�Ŀ¼
set DestPath=%CD%
cd /d "%~dp0"

rem ��⵱ǰϵͳƽ̨
set NowPlatform=x86
echo %ProgramW6432% | findstr 32
if not %errorlevel%==0 set NowPlatform=x64
echo ��ǰϵͳƽ̨ %NowPlatform%

rem ѡ���뵱ǰϵͳƽ̨ƥ���Lua����
set LuaExe=%LuaBasePath%\%NowPlatform%\lua
echo Lua·�� %LuaExe%

echo �ϲ��ļ�...

rem ��ʼ���ϲ��ű����ϲ�ע��
set xlua_ver=6.0
echo xlua_ver = %xlua_ver%>%DestLua%
echo xlua�汾 %xlua_ver%>%DestCmt%

rem �ϲ��ű�����ȡע��
for /r %%f in (Lua\*.lua) do echo     %%f& "%LuaExe%" -e "local file = io.open( [[%%f]], 'r'); local data = file:read('*a'); file:close(); local notes = ''; for note in data:gmatch('%%-%%-%%[=======%%[(.-)%%]=======%%]') do notes = notes .. '\r\n' .. note .. '\r\n'; end   file = io.open( '%DestLua%', 'a+'); file:write( '\r\ndo\r\n' .. data .. '\r\nend\r\n' ); file:close(); file = io.open( '%DestCmt%', 'a+'); file:write( notes ); file:close();"&if not %errorlevel%==0 echo echo !!!!!!!!�ϲ�ʧ��!!!!!!!!&&goto mkerror

echo �ϲ����

rem �������ã�ѡ����Ӧ����ƽ̨
set LuaPlatform=x64
if not "%1" == "64" set LuaPlatform=x86
echo ��ǰ���� %LuaPlatform% ...

rem ����ű�
"%LuaBasePath%\%LuaPlatform%\luac" -s -o %DestLua% %DestLua%

if not %errorlevel%==0 echo !!!!!!!!����ʧ��!!!!!!!!&&goto mkerror

echo �������

goto end

::����ָ���ǹ���д���Դ�ʹVS��������ж�
:mkerror
echo 2&>1
goto end

:end
echo ++++++++Ԥ�������++++++++
:eof
