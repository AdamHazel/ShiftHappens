QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addcar_d.cpp \
    addcustomer_d.cpp \
    assignment_d.cpp \
    editcar_d.cpp \
    editcustomer_d.cpp \
    exporter.cpp \
    helperfunctions.cpp \
    importer.cpp \
    main.cpp \
    mainwindow.cpp \
    shiftdatabase.cpp

HEADERS += \
    addcar_d.h \
    addcustomer_d.h \
    assignment_d.h \
    car.h \
    customer.h \
    editcar_d.h \
    editcustomer_d.h \
    exporter.h \
    helperfunctions.h \
    importer.h \
    mainwindow.h \
    shiftdatabase.h

FORMS += \
    addcar_d.ui \
    addcustomer_d.ui \
    assignment_d.ui \
    editcar_d.ui \
    editcustomer_d.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
