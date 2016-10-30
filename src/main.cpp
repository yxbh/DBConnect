#include <QApplication>
#include "Controllers/DBConnectMainWindow.hpp"


int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    DBConnectMainWindow dbconnect;
    dbconnect.show();

    return app.exec();
}
