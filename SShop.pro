#-------------------------------------------------
#
# Project created by QtCreator 2016-10-01T14:10:28
#
#-------------------------------------------------

QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SShop
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    authdialog.cpp \
    addpurchasedialog.cpp \
    addgooddialog.cpp \
    editgooddialog.cpp \
    WorkersDialog.cpp \
    AddWorkerDialog.cpp \
    EditWorkerDialog.cpp \
    ProvidersDialog.cpp \
    AddProviderDialog.cpp \
    EditProviderDialog.cpp \
    ServerSettingDialog.cpp \
    PostSettingDialog.cpp \
    AddPostDialog.cpp \
    EditPostDialog.cpp \
    access.cpp

HEADERS  += mainwindow.h \
    authdialog.h \
    addpurchasedialog.h \
    usertypes.h \
    addgooddialog.h \
    editgooddialog.h \
    WorkersDialog.h \
    AddWorkerDialog.h \
    EditWorkerDialog.h \
    ProvidersDialog.h \
    AddProviderDialog.h \
    EditProviderDialog.h \
    ServerSettingDialog.h \
    PostSettingDialog.h \
    AddPostDialog.h \
    EditPostDialog.h \
    access.h

FORMS    += mainwindow.ui \
    authdialog.ui \
    addpurchasedialog.ui \
    addgooddialog.ui \
    editgooddialog.ui \
    WorkersDialog.ui \
    AddWorkerDialog.ui \
    EditWorkerDialog.ui \
    ProvidersDialog.ui \
    AddProviderDialog.ui \
    EditProviderDialog.ui \
    ServerSettingDialog.ui \
    PostSettingDialog.ui \
    AddPostDialog.ui \
    EditPostDialog.ui

RESOURCES += \
    icons.qrc \
    rpt.qrc

QMAKE_CXXFLAGS += -std=gnu++14

include($$PWD/QtRptProject/QtRPT/QtRPT.pri)
