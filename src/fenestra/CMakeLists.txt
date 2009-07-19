set(FENESTRA_SOURCES main.cpp application.cpp client.cpp frame.cpp)
qt4_automoc(${FENESTRA_SOURCES})

add_executable(fenestra ${FENESTRA_SOURCES})
target_link_libraries(fenestra ${QT_LIBRARIES} ${X11_LIBRARIES})
