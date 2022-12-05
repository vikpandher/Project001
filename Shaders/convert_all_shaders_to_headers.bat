@ECHO OFF

SETLOCAL ENABLEDELAYEDEXPANSION
FOR /f %%G IN ('DIR *.vert *.tesc *.tese *.geom *.frag *.comp /b') DO (
    SET x=%%~xG
    SET y=%%~nG_!x:~1!
    SET z=static const char* g_!y! = R"^(
    ECHO !z! > !y!.h
    FOR /f "tokens=* delims=," %%X IN (%%G) DO (
        ECHO %%X >> !y!.h
    )
    SET a=^)"
    ECHO !a! >> !y!.h;
)