include(VBE/VBE.pro)
INCLUDEPATH += .
#DEFINES += __DLOG
LIBS += -lBox2D
SOURCES += main.cpp \
	commons.cpp \
	SceneMain/SceneMain.cpp \
	SceneMain/DeferredContainer.cpp \
	SceneMain/DeferredLight.cpp \
	SceneMain/BlurContainer.cpp \
	SceneMain/SquareObject.cpp \
	SceneMain/Player.cpp \
	SceneMain/physics/PhysicsEngine.cpp \
	SceneMain/physics/PhysicsBody.cpp

HEADERS += \
	commons.hpp \
	SceneMain/SceneMain.hpp \
	SceneMain/DeferredContainer.hpp \
	SceneMain/DeferredLight.hpp \
	SceneMain/BlurContainer.hpp \
	SceneMain/SquareObject.hpp \
	SceneMain/Player.hpp \
	SceneMain/physics/PhysicsEngine.hpp \
	SceneMain/physics/PhysicsBody.hpp

OTHER_FILES += \
	data/shaders/quad.vert \
	data/shaders/light.frag \
	data/shaders/blurPassVertical.frag \
	data/shaders/blurPassHoritzontal.frag \
	data/shaders/blurMaskPass.frag \
	data/shaders/quad.frag \
	data/shaders/depth.frag \
	data/shaders/depth.vert \
	data/shaders/standardDeferred.vert \
	data/shaders/standardDeferred.frag \
	data/shaders/ambientPass.frag

