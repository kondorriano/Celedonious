QT -= gui

TARGET = Celedonious
CONFIG -= app_bundle

TEMPLATE = app

include(../VBE-Physics2D/VBE-Physics2D.pri)
include(../VBE-Scenegraph/VBE-Scenegraph.pri)
include(../VBE/VBE.pri)
include(../common.pri)

INCLUDEPATH += .

SOURCES += main.cpp \
	commons.cpp \
	SceneMain/SceneMain.cpp \
	SceneMain/DeferredContainer.cpp \
	SceneMain/DeferredLight.cpp \
	SceneMain/BlurContainer.cpp \
        SceneMain/Player.cpp \
    SceneMain/GenericBody.cpp \
    SceneMain/MyDebugDrawer.cpp \
    SceneMain/Manager.cpp \
    SceneMain/CeledoniousSystem.cpp

HEADERS += \
	commons.hpp \
	SceneMain/SceneMain.hpp \
	SceneMain/DeferredContainer.hpp \
	SceneMain/DeferredLight.hpp \
	SceneMain/BlurContainer.hpp \
        SceneMain/Player.hpp \
    SceneMain/GenericBody.hpp \
    SceneMain/MyDebugDrawer.hpp \
    SceneMain/Manager.hpp \
    SceneMain/CeledoniousSystem.hpp

OTHER_FILES += \
	assets/shaders/light.frag \
	assets/shaders/blurPassVertical.frag \
	assets/shaders/blurPassHoritzontal.frag \
	assets/shaders/blurMaskPass.frag \
	assets/shaders/quad.frag \
	assets/shaders/depth.frag \
	assets/shaders/depth.vert \
	assets/shaders/standardDeferred.vert \
	assets/shaders/standardDeferred.frag \
	assets/shaders/ambientPass.frag \
    assets/shaders/debug.frag



