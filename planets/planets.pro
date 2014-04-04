QT += widgets declarative

TEMPLATE = lib
DESTDIR = $$_PRO_FILE_PWD_/../bin
include(planets.pri)

# library dependencies
LIBS += -L$$_PRO_FILE_PWD_/../bin
LIBS += -lastroprocessor



































