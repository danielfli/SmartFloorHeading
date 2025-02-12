file(GLOB APP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*")

add_executable(${EXECUTABLE_NAME} ${APP_SOURCES})

# Output bcm2835 Lib
find_library(bcm2835 /usr/local/lib/)


find_package(Boost COMPONENTS program_options REQUIRED)
if(Boost_FOUND)
    message(STATUS "Found Boost Library")
    # include_directories(${Boost_INCLUDE_DIRS})
    # target_link_libraries(foo ${Boost_LIBRARIES})
else()
    message(FATAL_ERROR "No Boost Library found!")
endif()


target_include_directories(${LIBRARY_NAME} PUBLIC
    ${CMAKE_BINARY_DIR}
    ${Boost_INCLUDE_DIRS}
    ${CMAKE_PROJECT_DIR}/include/smartfloorheating
    ${CMAKE_PROJECT_DIR}/external
)

target_link_libraries(${EXECUTABLE_NAME} PUBLIC
    ${LIBRARY_NAME}
    ${LIBRARY_HEADERS}
    ${Boost_LIBRARIES}
    ${CURL_LIBRARIES}
    bcm2835.a
    ${OPENSSL_CRYPTO_LIBRARY}
)


if (${ENABLE_WARNINGS})
    target_set_warnings(TARGET ${EXECUTABLE_NAME} ENABLE ON AS_ERROR OFF)
endif()