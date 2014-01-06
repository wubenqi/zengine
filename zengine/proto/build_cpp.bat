@set out_src=..\..\cc\common
@set language=cpp
@set import=-I.\

@setlocal enabledelayedexpansion
@echo off
set OLD_PWD=%CD%
CD %out_src%
FOR %%f IN (*.pb.h) DO (
	set head_file=%%f
	set cpp_file=!head_file:.pb.h=.pb.cc!
	set proto_file=!head_file:.pb.h=.proto!
	if not exist !OLD_PWD!\!proto_file! (
		echo Del file: !head_file! !cpp_file!
		DEL !head_file!
		DEL !cpp_file!
	)
)
CD %OLD_PWD%

FOR %%f IN (*.proto) DO (
	set t=%%f
	echo %%f
	set cpp_file=!t:.proto=.pb.cc!
	set head_file=!t:.proto=.pb.h!
	if exist !out_src!\!cpp_file! (
		dir /T:W !out_src!\!cpp_file! | findstr "^[0-9][0-9][0-9][0-9]-">temp
		set /P file1=<temp
		set time1=!file1:~0,17!
		set time1=!time1: =!
		dir /T:W %%f | findstr "^[0-9][0-9][0-9][0-9]-">temp
		set /p file2=<temp
		set time2=!file2:~0,17!
		set time2=!time2: =!
		if !time1! LEQ !time2! (
			echo --^> !head_file! !cpp_file!
			..\..\protoc\protoc %import% --%language%_out=%out_src% %%f
		)
		del temp
	) else (
		echo --^> !head_file! !cpp_file!
		..\..\protoc\protoc %import% --%language%_out=%out_src% %%f
	)
)
pause
