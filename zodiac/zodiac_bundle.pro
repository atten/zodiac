QT += widgets network declarative
DESTDIR = $$_PRO_FILE_PWD_/../bin
TARGET = zodiac
TEMPLATE = app

VPATH += ../swe ../astroprocessor ../chart ../fileeditor ../plain ../planets ../details

include(../swe/swe.pri)
include(../astroprocessor/astroprocessor.pri)
include(../chart/chart.pri)
include(../fileeditor/fileeditor.pri)
include(../plain/plain.pri)
include(../planets/planets.pri)
include(../details/details.pri)
include(zodiac.pri)

#CONFIG(release, debug|release): DEFINES += QT_NO_DEBUG_OUTPUT
