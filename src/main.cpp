#include <QApplication>
#include "Config.hpp"
#include "Controllers/DBConnectMainWindow.hpp"
#include "appinfo.hpp"
#include "version.hpp"

int main(int argc, char ** argv)
{
    QApplication::setOrganizationName(app::info::COMPANY);
    QApplication::setApplicationName(app::info::PRODUCT);
    QApplication::setApplicationVersion(app::version::FULL);
    Config::initialise();

    QApplication app(argc, argv);

    DBConnectMainWindow dbconnect;
    dbconnect.show();

    return app.exec();
}
