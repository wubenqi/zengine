@echo off
cd ..\..\tools
message_handler_tool2 --app=server
copy luabind_register_helper_pb.cc ..\luabind\server
cd ..\proto\server
pause
