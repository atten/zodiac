#-------------------------------------------------
#
# Project created by QtCreator 2012-01-03T01:27:17
#
#-------------------------------------------------

QT += widgets network declarative

TARGET = zodiac_part
TEMPLATE = app
DESTDIR = $$_PRO_FILE_PWD_/../bin
include(zodiac.pri)

# library dependencies
LIBS += -L$$_PRO_FILE_PWD_/../bin
LIBS += -lastroprocessor -lfileeditor -lplain -lchart -lplanets -ldetails

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
