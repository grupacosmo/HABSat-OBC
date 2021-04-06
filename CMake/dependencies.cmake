include(FetchContent)

FetchContent_Declare(GSL
        GIT_REPOSITORY https://github.com/microsoft/GSL
        GIT_TAG main
        )

FetchContent_MakeAvailable(GSL)