set PATH=%PATH%;C:\Qt\4.8.7\bin;C:\sw\mingw32\bin
del /q release\*.exe
qmake project.pro
make -f Makefile.Release 2>error.txt
strip Release\iwscope.exe
copy c:\sw\gsl\bin\* bin /y
copy ..\fixed_lib\bin\*.* bin  /y
copy ..\extlib\bin\extlib.dll bin  /y
copy ..\extlib\bin\extlib.dll . /y
