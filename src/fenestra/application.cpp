#include "application.h"

Application::Application(int& argc, char** argv)
: QApplication(argc, argv)
{}

bool Application::x11EventFilter(XEvent* event)
{
    return false;
}

#include "application.moc"
