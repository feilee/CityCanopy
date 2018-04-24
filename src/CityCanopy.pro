#-------------------------------------------------
#
# Project created by QtCreator 2015-04-06T13:28:56
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 5):

TARGET = CityCanopy
TEMPLATE = app

SOURCES += main.cpp\
    mainwindow.cpp \
    io/topodatamanager.cpp \
    io/urbandatamanager.cpp \
    io/osmdatamanager.cpp \
    io/write_geogrid.c \
    io/exporter.cpp \
    io/wrfexporter.cpp \
    viz/gl/figure.cpp \
    viz/gl/glwidget.cpp \
    viz/gl/topofigure.cpp \
    viz/gl/urbanfigure.cpp \
    viz/gl/osmfigure.cpp

HEADERS  += mainwindow.h \
    datamanager.h \
    io/topodatamanager.h \
    io/urbandatamanager.h \
    io/osmdatamanager.h \
    io/exporter.h \
    io/wrfexporter.h \
    math/cuda_code.cuh \
    math/random_generator.hpp \
    viz/gl/glwidget.h \
    viz/gl/figure.h \
    viz/gl/topofigure.h \
    viz/gl/urbanfigure.h \
    viz/gl/osmfigure.h \
    ../third_party/others/OpenGlCudaHelper.h



FORMS    += \
    viz/qt/mainwindow.ui

DISTFILES +=

RESOURCES += \
    viz/qt/resources.qrc

# Cuda sources
CUDA_SOURCES += math/cuda_code.cu

# Path to cuda toolkit install
CUDA_DIR      = /opt/cuda
# Path to header and libs files
INCLUDEPATH  += $$CUDA_DIR/include \
                math \
                ../src \
                /usr/include/qt5/
QMAKE_LIBDIR += $$CUDA_DIR/lib64     # Note I'm using a 64 bits Operating system
# libs used in your code
LIBS += -lcudart -lcuda
# GPU architecture
CUDA_ARCH     = sm_60                # Yeah! I've a new device. Adjust with your compute capability
# Here are some NVCC flags I've always used by default.
NVCCFLAGS     = --compiler-options -fno-strict-aliasing -use_fast_math --ptxas-options=-v

# Prepare the extra compiler configuration (taken from the nvidia forum - i'm not an expert in this part)
CUDA_INC = $$join(INCLUDEPATH,' -I','-I',' ')

cuda.commands = $$CUDA_DIR/bin/nvcc -m64 -O3 -arch=$$CUDA_ARCH -c $$NVCCFLAGS \
                $$CUDA_INC $$LIBS  ${QMAKE_FILE_NAME} -o ${QMAKE_FILE_OUT} \
                2>&1 | sed -r \"s/\\(([0-9]+)\\)/:\\1/g\" 1>&2
# nvcc error printout format ever so slightly different from gcc
# http://forums.nvidia.com/index.php?showtopic=171651

cuda.dependency_type = TYPE_C # there was a typo here. Thanks workmate!
cuda.depend_command = $$CUDA_DIR/bin/nvcc -O3 -M $$CUDA_INC $$NVCCFLAGS   ${QMAKE_FILE_NAME}

cuda.input = CUDA_SOURCES
cuda.output = ${OBJECTS_DIR}${QMAKE_FILE_BASE}_cuda.o
# Tell Qt that we want add more stuff to the Makefile
QMAKE_EXTRA_COMPILERS += cuda
