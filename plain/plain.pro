QT += widgets
TEMPLATE = lib
DESTDIR = $$_PRO_FILE_PWD_/../bin
include(plain.pri)

# library dependencies
LIBS += -L$$_PRO_FILE_PWD_/../bin
LIBS += -lastroprocessor































