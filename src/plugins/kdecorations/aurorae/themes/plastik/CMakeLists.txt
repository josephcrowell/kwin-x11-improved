add_subdirectory(code)

install(DIRECTORY package/
        DESTINATION ${KDE_INSTALL_DATADIR}/${KWIN_DATADIR}/decorations/kwin4_decoration_qml_plastik)

file(COPY package/ DESTINATION ${CMAKE_BINARY_DIR}/bin/${KWIN_DATADIR}/decorations/kwin4_decoration_qml_plastik)
