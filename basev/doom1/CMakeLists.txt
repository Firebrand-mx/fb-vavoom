configure_file(basepak.ls.in ${CMAKE_CURRENT_BINARY_DIR}/basepak.ls)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/basepak.pk3
	COMMAND ${VLUMPY} ARGS basepak.ls
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/basepak.ls vlumpy
		basepak.ls.in
		mapinfo.txt
		sndinfo.txt
	)
add_custom_target(doom1_basepak ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/basepak.pk3)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/basepak.pk3 base.txt
	DESTINATION ${DATADIR}/basev/doom1)
