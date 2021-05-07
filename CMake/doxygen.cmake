find_package(Doxygen)

if (DOXYGEN_FOUND)
    set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md")
    set(DOXYGEN_HTML_EXTRA_STYLESHEET
            "${CMAKE_SOURCE_DIR}/Docs/doxygen-awesome-css/doxygen-awesome.css"
            "${CMAKE_SOURCE_DIR}/Docs/doxygen-awesome-css/doxygen-awesome-sidenav.css")
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_EXTRACT_ALL YES)

    doxygen_add_docs(docs
            ${CMAKE_SOURCE_DIR}/Lib
            ${CMAKE_SOURCE_DIR}/Inc
            ${CMAKE_SOURCE_DIR}/Src
            )
endif (DOXYGEN_FOUND)