#include <clientwindow.h>
#include "application.h"

int main(int argc, char** argv)
{
    Application app(argc, argv);
    if (app.createClientWindow()) {
        return app.exec();
    } else {
        return 1;
    }
}

