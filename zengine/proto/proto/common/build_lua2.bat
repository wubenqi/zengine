@set out_src=..\..\luabind\common
@set language=lua
@set import=-I.\


@set path=%path%;%protobuf_lib%
@echo off
FOR   %%f   IN   (*.proto)   DO   (   
  echo %%f
  ..\..\protoc\protoc %import% --plugin=protoc-gen-lua="..\..\protoc\protoc-gen-lua2.bat" --%language%_out=%out_src% %%f 
)

pause