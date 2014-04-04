#-------------------------------------------------
#
# Project created by QtCreator 2013-03-27T01:47:20
#
#-------------------------------------------------

QT += widgets
TARGET = astroprocessor
TEMPLATE = lib
DESTDIR = $$_PRO_FILE_PWD_/../bin
include(astroprocessor.pri)

## library dependencies

LIBS += -L$$_PRO_FILE_PWD_/../bin
LIBS += -lswe


