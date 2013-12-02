#
# hershey-fonts - Copyright (C) 2013 Kamal Mostafa <kamal@whence.com>
#
# License: GPL-2+
#

cmake_minimum_required(VERSION 2.6)

set(HERSHEY_FONTS_DIR	"${CMAKE_INSTALL_PREFIX}/share/hershey-fonts")

add_definitions(-DHERSHEY_FONTS_DIR="${HERSHEY_FONTS_DIR}")

add_library(hersheyfont			SHARED hersheyfont.c)
set_target_properties(hersheyfont	PROPERTIES VERSION 0 SOVERSION 0)
install(TARGETS hersheyfont		LIBRARY DESTINATION lib)
add_library(hersheyfontStatic		STATIC hersheyfont.c)
set_target_properties(hersheyfontStatic	PROPERTIES OUTPUT_NAME hersheyfont)
install(TARGETS hersheyfontStatic	ARCHIVE DESTINATION lib)
install(FILES hersheyfont.h		DESTINATION include)


include_directories (.)
link_directories (${CMAKE_BINARY_DIR})

add_executable(hershey-font-example		hershey-font-example.c)
target_link_libraries(hershey-font-example	hersheyfont)
# install(TARGETS hershey-font-example		DESTINATION bin)
install(FILES hershey-font-example.c
				DESTINATION share/doc/libhersheyfont/examples)

add_custom_target(libhersheyfont.3 ALL
    COMMAND ${CMAKE_COMMAND} -E
	copy ${CMAKE_CURRENT_SOURCE_DIR}/libhersheyfont.3 libhersheyfont.3
    DEPENDS libhersheyfont.3
)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libhersheyfont.3
						DESTINATION share/man/man3)

set(manpage_symlinks	hershey_font_load.3 hershey_font_jhf_load.3 hershey_font_free.3 hershey_font_glyph.3 )

foreach(manpage ${manpage_symlinks})
    add_custom_target(${manpage} ALL
	COMMAND ${CMAKE_COMMAND} -E create_symlink libhersheyfont.3 ${manpage}
	DEPENDS libhersheyfont.3
    )
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${manpage}
						DESTINATION share/man/man3)
endforeach(manpage)

