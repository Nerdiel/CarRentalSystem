cmake_minimum_required (VERSION 3.12)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project("CarRentalSystem")

# Specify output directory for executable
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

# Set the output directory for CMake-generated files
set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}/cmake_build)

# Specify the location for CMake's internal data (e.g., CMakeCache.txt)
set(CMAKE_CACHEFILE_DIR ${CMAKE_CURRENT_BINARY_DIR}/cmake_build)


# Specify output directory for files
set(FILE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_CXX_STANDARD 20)
add_executable (CarRentalSystem "CarRentalSystem.cpp" "CarRentalSystem.h" "FileHandler.h" "FileHandler.cpp" "Objects.cpp" "Objects.h" "ConsoleController.cpp" "System.h" "System.cpp")

# Create directories in the build directory
file(MAKE_DIRECTORY ${FILE_OUTPUT_PATH}/src)
file(MAKE_DIRECTORY ${FILE_OUTPUT_PATH}/src/Cars ${FILE_OUTPUT_PATH}/src/Customers ${FILE_OUTPUT_PATH}/src/Customers/Contracts ${FILE_OUTPUT_PATH}/src/Customers/Contracts/Active ${FILE_OUTPUT_PATH}/src/Customers/Contracts/Archived ${FILE_OUTPUT_PATH}/src/Users)

# Define the list of filenames relative to the build directory
set(filenameslist "${FILE_OUTPUT_PATH}/src/Cars/available.txt"
                  "${FILE_OUTPUT_PATH}/src/Cars/rented.txt"
                  "${FILE_OUTPUT_PATH}/src/Cars/repair_shop.txt"
                  "${FILE_OUTPUT_PATH}/src/Cars/permanently_unavailable.txt"
                  "${FILE_OUTPUT_PATH}/src/Customers/customers.txt"
                  "${FILE_OUTPUT_PATH}/src/Users/users.txt")

# Create files in the build directory
foreach(filename IN LISTS filenameslist)
    if(NOT EXISTS ${filename})
        if(${filename} MATCHES ".*customers.txt")
            file(WRITE ${filename} "NAME#SURNAME#EMAIL#PHONE#ADDRESS\n")
        elseif(${filename} MATCHES ".*users.txt")
            file(WRITE ${filename} "USERNAME#PASSWORD#ADMIN\nadmin#1234#1\n")
        else()
            file(WRITE ${filename} "MAKE#MODEL#YEAR#COLOR#LICENCE_PLATE#MOTORIZATION#GEARBOX#SEATS#COST_PER_HOUR\n")
        endif()
    endif()
endforeach()
