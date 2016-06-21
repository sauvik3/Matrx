@echo on
@for %%X in (cl.exe) do (set CLFOUND=%%~$PATH:X)
if not defined CLFOUND (
    call "C:\Program Files\Microsoft Visual Studio 14.0\VC\vcvarsall.bat"
)

@cl.exe /c /O2 /EHsc /MD /I ".\include" ".\src\matrix_io.cpp" /Fo".\obj\matrix_io.obj"
@cl.exe /c /O2 /EHsc /MD /I ".\include" ".\src\main.cpp" /Fo".\obj\main.obj"
@rc.exe -fo ".\rc\main.res" ".\rc\main.rc"

@link.exe /MACHINE:X86 /nodefaultlib:clib.lib /OPT:NOREF /entry:mainCRTStartup /subsystem:console /out:".\bin\main.exe" ".\lib\matrix.lib" ".\obj\matrix_io.obj" ".\obj\main.obj" ".\rc\main.res"