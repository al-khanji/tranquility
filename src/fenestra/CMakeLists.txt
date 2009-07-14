set(FENESTRA_SOURCES main.cpp application.cpp)
qt4_automoc(${FENESTRA_SOURCES})

add_executable(fenestra ${FENESTRA_SOURCES})
target_link_libraries(fenestra ${QT_LIBRARIES} X11)
