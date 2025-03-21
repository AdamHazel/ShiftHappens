QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addcar_d.cpp \
    addcustomer_d.cpp \
    addrental_d.cpp \
    editcar_d.cpp \
    editcustomer_d.cpp \
    helper_functions.cpp \
    importexporter.cpp \
    main.cpp \
    mainwindow.cpp \
    shiftdatabase.cpp

HEADERS += \
    addcar_d.h \
    addcar_d.h \
    addcustomer_d.h \
    addrental_d.h \
    car.h \
    customer.h \
    editcar_d.h \
    editcustomer_d.h \
    helper_functions.h \
    importexporter.h \
    mainwindow.h \
    rental.h \
    shiftdatabase.h

FORMS += \
    addcar_d.ui \
    addcustomer_d.ui \
    addrental_d.ui \
    editcar_d.ui \
    editcustomer_d.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
