#include <QtCore>
#include <iostream>

#include "qwebdav.cpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QWebDav wd;
    wd.connectToHost("192.168.40.118", 8888, "", "");
    
    std::cout << "asd" << std::endl;
}