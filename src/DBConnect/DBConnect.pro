#-------------------------------------------------
#
#   DBConnect QMake file.
#   Author: Benjamin YanXiang Huang
#
#-------------------------------------------------

QT  += core         # core, always needed.
QT  += widgets      # GUI
QT  += gui          # for developing GUI apps.
QT  += sql          # database connection

# executable name
TARGET = DBConnect

TEMPLATE = app

#
# Configure release & debug build directories.
#
BUILDDIRWIN32DBG = ../../build-output/DBConnect-Win32-Debug
BUILDDIRWIN32REL = ../../build-output/DBConnect-Win32-Release
BUILDDIRUNIXDBG  = ../../build-output/DBConnect-Unix-Debug
BUILDDIRUNIXREL  = ../../build-output/DBConnect-Unix-Release
CONFIG(debug, debug|release) { # Debug build dirs
    win32:DESTDIR       = $${BUILDDIRWIN32DBG}/
    win32:OBJECTS_DIR   = $${BUILDDIRWIN32DBG}/obj
    win32:MOC_DIR       = $${BUILDDIRWIN32DBG}/moc
    win32:RCC_DIR       = $${BUILDDIRWIN32DBG}/rsc
    win32:UI_DIR        = $${BUILDDIRWIN32DBG}/ui
    unix:DESTDIR        = $${BUILDDIRUNIXDBG}/obj
    unix:OBJECTS_DIR    = $${BUILDDIRUNIXDBG}/obj
    unix:MOC_DIR        = $${BUILDDIRUNIXDBG}/moc
    unix:RCC_DIR        = $${BUILDDIRUNIXDBG}/rsc
    unix:UI_DIR         = $${BUILDDIRUNIXDBG}/ui
} else { # Release build dirs
    win32:DESTDIR       = $${BUILDDIRWIN32REL}/
    win32:OBJECTS_DIR   = $${BUILDDIRWIN32REL}/obj
    win32:MOC_DIR       = $${BUILDDIRWIN32REL}/moc
    win32:RCC_DIR       = $${BUILDDIRWIN32REL}/rsc
    win32:UI_DIR        = $${BUILDDIRWIN32REL}/ui
    unix:DESTDIR        = $${BUILDDIRUNIXREL}/
    unix:OBJECTS_DIR    = $${BUILDDIRUNIXREL}/obj
    unix:MOC_DIR        = $${BUILDDIRUNIXREL}/moc
    unix:RCC_DIR        = $${BUILDDIRUNIXREL}/rsc
    unix:UI_DIR         = $${BUILDDIRUNIXREL}/ui
}
#

#
# Does this any request features? Still not sure.
#
QMAKE_CXXFLAGS_WARN_ON
QMAKE_CXXFLAGS_THREAD
#

QMAKE_CXXFLAGS_WARN_OFF += -Wunused-function

#CONFIG += console
CONFIG += c++11

#
# libraries
#
unix:LIBS += -L/usr/lib/x86_64-linux-gnu/mesa/

#
# source files
#
SOURCES += \
    ../main.cpp \
    ../DBConnectMainWindow.cpp \
    ../AboutDialog.cpp \
    ../ChangeLogDialog.cpp

FORMS += \
    ../DBConnectMainWindow.ui \
    ../AboutDialog.ui \
    ../ChangeLogDialog.ui

HEADERS += \
    ../DBConnectMainWindow.hpp \
    ../AboutDialog.hpp \
    ../ChangeLogDialog.hpp

