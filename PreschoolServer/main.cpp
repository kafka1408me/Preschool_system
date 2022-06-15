#include "application.h"
#include "server.h"

int main(int argc, char** argv)
{
    Application app(argc, argv);

    int port = app.getServerPort();
    Server server(port);
    server.start();

    return app.exec();
}
