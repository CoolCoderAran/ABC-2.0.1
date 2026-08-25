set HOME=%homedrive%%homepath%\
set TERM=cygwin
set SCREEN=bios 50 80
set TERMINFO=notfound
IF EXIST %homedrive%\Program Files\ABC\terminfo set TERMINFO=%homedrive%\Program Files\ABC\terminfo
IF EXIST %homedrive%\Program Files (x86)\ABC\terminfo set TERMINFO=%homedrive%\Program Files (x86)\ABC\terminfo
IF EXIST %CD%\ABC\terminfo set TERMINFO=%CD%\ABC\terminfo
IF EXIST %CD%\terminfo set TERMINFO=%CD%\terminfo
IF %TERMINFO% == notfound Pause TERMINFO not found
abcexe.exe
pause

