option(CPP_CHECK "use Cpp Check while compiling, if available" OFF)

if (CPP_CHECK)
    find_program(CPP_CHECK_EXEC "cppcheck")
    if(CPP_CHECK_EXEC)
        message(STATUS "Using Cpp Check")
      set(CMAKE_CXX_CPPCHECK ${CPP_CHECK_EXEC})
    else()
        find_program(HAS_APT "apt" NO_CACHE)
        if(HAS_APT)
            message(WARNING "Requested Cpp Check, but it was not found. Continuing without it. You can install it using:\nsudo apt install cppcheck\n")
        else()
            message(WARNING "Requested Cpp Check, but it was not found. Continuing without it. You can install it from here:\nhttps://cppcheck.sourceforge.io/")
        endif()
    endif()
endif()