# Simple CMakeLists.txt to build LuaSec as a static library (assuming Lua is a static library as well)
cmake_minimum_required(VERSION 2.4.4...4.0.0)
project (LuaSec C)

if(MSVC)
	add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
	add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
endif()





# Store the Lua files as C++11 string constants:
set(LUAMODULES
	https.lua
	ssl.lua
)

set(GenLuaFiles)
foreach(LuaFile IN LISTS LUAMODULES)
	string(REPLACE ".lua" "" LuaFileBare ${LuaFile})
	file(READ src/${LuaFile} LuaFileContents)
	set(outfile ${CMAKE_CURRENT_BINARY_DIR}/${LuaFile}.cpp)
	file(WRITE ${outfile} "extern \"C\" const char LuaSrc_${LuaFileBare}[] = R\"%%%%(" ${LuaFileContents} ")%%%%\";")
	list(APPEND GenLuaFiles ${outfile})
endforeach()





# Static library:
add_library(LuaSec-static STATIC
	"src/options.c"
	"src/config.c"
	"src/ec.c"
	"src/x509.c"
	"src/context.c"
	"src/ssl.c"
	
	# The CPP files generated from LUA sources:
	${GenLuaFiles}
	
	# Static registering of the generated CPP files:
	src/staticRegister.cpp
)

target_link_libraries(LuaSec-static
	lua-static
	luasocket-static
	OpenSSL::Crypto
	OpenSSL::SSL
)

target_include_directories(LuaSec-static
	SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src
)
