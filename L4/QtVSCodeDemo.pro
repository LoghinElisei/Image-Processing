QT += widgets
CONFIG += c++17 console
TARGET = QtVSCodeDemo
TEMPLATE = app

SOURCES += \
    Lab2_PIM.cpp \
    ImageGrid.cpp \
    Tools.cpp\
    ImageViewer.cpp\
    HistogramViewer.cpp\
    AbstractViewer.cpp\
    ImageGridCell.cpp
    


HEADERS += \
    ImageGrid.h\
    ImageViewer.h\
    Tools.h\
    operatii.h\
    ImageGridCell.h