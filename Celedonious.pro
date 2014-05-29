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
	SceneMain/physics/PhysicsBody.cpp \
    SceneMain/physics/Collider.cpp \
    SceneMain/physics/CircleCollider.cpp \
    SceneMain/physics/PolygonCollider.cpp \
    SceneMain/Level.cpp \
    SceneMain/DebugDrawer.cpp

HEADERS += \
	commons.hpp \
	SceneMain/SceneMain.hpp \
	SceneMain/DeferredContainer.hpp \
	SceneMain/DeferredLight.hpp \
	SceneMain/BlurContainer.hpp \
	SceneMain/SquareObject.hpp \
	SceneMain/Player.hpp \
	SceneMain/physics/PhysicsEngine.hpp \
	SceneMain/physics/PhysicsBody.hpp \
    SceneMain/physics/Collider.hpp \
    SceneMain/physics/CircleCollider.hpp \
    SceneMain/physics/PolygonCollider.hpp \
    SceneMain/Level.hpp \
    SceneMain/physics/PhysicsDebugDrawer.hpp \
    SceneMain/DebugDrawer.hpp

OTHER_FILES += \
	data/shaders/light.frag \
	data/shaders/blurPassVertical.frag \
	data/shaders/blurPassHoritzontal.frag \
	data/shaders/blurMaskPass.frag \
	data/shaders/quad.frag \
	data/shaders/depth.frag \
	data/shaders/depth.vert \
	data/shaders/standardDeferred.vert \
	data/shaders/standardDeferred.frag \
	data/shaders/ambientPass.frag \
    data/shaders/debug.frag

