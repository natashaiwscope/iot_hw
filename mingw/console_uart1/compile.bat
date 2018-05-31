set PATH=%PATH%;C:\Qt\4.8.7\bin;C:\sw\mingw32\bin
del /q release\*.exe
qmake project.pro
make
make -f Makefile.Release 2>error.txt
strip Release\iwscope.exe
copy c:\sw\gsl\bin\* bin /y
copy ..\udplib\bin\*.* bin  /y
copy ..\..\lib\win32\*.* bin  /y
