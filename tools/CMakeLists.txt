#
# hershey-fonts - Copyright (C) 2013 Kamal Mostafa <kamal@whence.com>
#
# License: GPL-2+
#

cmake_minimum_required(VERSION 2.6)

include_directories (../libhersheyfont)
link_directories (${CMAKE_BINARY_DIR}/../libhersheyfont)

add_executable(hershey-font-check		hershey-font-check.c)
target_link_libraries(hershey-font-check	hersheyfont)
# install(TARGETS hershey-font-check		DESTINATION bin)

add_executable(hershey-font-gnuplot		hershey-font-gnuplot.c)
target_link_libraries(hershey-font-gnuplot	hersheyfont)
install(TARGETS hershey-font-gnuplot		DESTINATION bin)
install(FILES hershey-font-gnuplot.1		DESTINATION share/man/man1)
