TEMPLATE = app

QT += qml quick quickcontrols2 concurrent svg network websockets bluetooth
CONFIG += link_pkgconfig

#Use this flag to disable qDebug to console
#DEFINES += QT_NO_DEBUG_OUTPUT
#App versioning
VERSION_MAJOR = 1
VERSION_MINOR = 6
VERSION_BUILD = 1

DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
       "VERSION_MINOR=$$VERSION_MINOR"\
       "VERSION_BUILD=$$VERSION_BUILD"

#Target version
DEFINES += APP_VER=\\\"$${VERSION_MAJOR}.$${VERSION_MINOR}.$${VERSION_BUILD}\\\"

CONFIG += c++11

DEFINES += REAL_VERSION

SOURCES += main.cpp \
    network/httpbase.cpp \
    network/httpparams.cpp \
    network/httptool.cpp \
    network/wsclient.cpp \
    network/httpbase2.cpp \
    utility/json.cpp \
    utility/logger.cpp \
    commonfunction.cpp \
    maincontroller.cpp \
    configsetting.cpp \
    keepwake.cpp \
    utils.cpp \
    bleinterface.cpp \
    blemanager.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    network/httpbase.h \
    network/httpparams.h \
    network/httptool.h \
    network/wsclient.h \
    network/httpbase2.h \
    utility/json.h \
    utility/logger.h \
    commonfunction.h \
    datatypes.h \
    maincontroller.h \
    configsetting.h \
    keepwake.h \
    utils.h \
    bleinterface.h \
    blemanager.h

android {
    QT += androidextras
    DEFINES += AUTO_HIDE_NAVI
}

RESOURCES += icons/icons.qrc

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
