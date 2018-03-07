FIND_PROGRAM(GETTEXT_MSGFMT_EXECUTABLE msgfmt)

IF(NOT GETTEXT_MSGFMT_EXECUTABLE)
	MESSAGE(
"------
                 NOTE: msgfmt not found. Translations will *not* be generated, just copied from package
------")
ENDIF(NOT GETTEXT_MSGFMT_EXECUTABLE)


SET(catalogname knoda4)
ADD_CUSTOM_TARGET(translations ALL)
FILE(GLOB PO_FILES *.po)
FOREACH(_poFile ${PO_FILES})

    GET_FILENAME_COMPONENT(_poFileName ${_poFile} NAME)
    STRING(REGEX REPLACE "^${catalogname}_?" "" _langCode ${_poFileName} )
    STRING(REGEX REPLACE "\\.po$" "" _langCode ${_langCode} )

    IF( _langCode )
        GET_FILENAME_COMPONENT(_lang ${_poFile} NAME_WE)
        SET(_gmoFile ${CMAKE_CURRENT_BINARY_DIR}/${_lang}.gmo)

        IF(GETTEXT_MSGFMT_EXECUTABLE)
            ADD_CUSTOM_COMMAND(TARGET translations
                COMMAND ${GETTEXT_MSGFMT_EXECUTABLE} -o ${_gmoFile} ${_poFile}
                DEPENDS ${_poFile})
        ELSE(GETTEXT_MSGFMT_EXECUTABLE)
            ADD_CUSTOM_COMMAND(TARGET translations
                COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${_lang}.gmo ${CMAKE_CURRENT_BINARY_DIR}
                DEPENDS ${_poFile})
        ENDIF(GETTEXT_MSGFMT_EXECUTABLE)

        INSTALL(FILES ${_gmoFile} DESTINATION ${LOCALE_INSTALL_DIR}/${_langCode}/LC_MESSAGES/ RENAME ${catalogname}.mo)
    ENDIF( _langCode )

ENDFOREACH(_poFile ${PO_FILES})


