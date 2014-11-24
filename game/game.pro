QT -= gui

TARGET = VoxelGame
CONFIG -= app_bundle

TEMPLATE = app

include(../VBE/VBE.pri)
include(../VBE-Scenegraph/VBE-Scenegraph.pri)
include(../common.pri)

INCLUDEPATH += .

SOURCES += main.cpp \
	commons.cpp \
	SceneMain/SceneMain.cpp \
	SceneMain/DeferredContainer.cpp \
	SceneMain/DeferredLight.cpp \
	SceneMain/BlurContainer.cpp \
	SceneMain/Player.cpp \
    SceneMain/physics/Collider.cpp \
    SceneMain/physics/CircleCollider.cpp \
    SceneMain/physics/PolygonCollider.cpp \
    SceneMain/GenericBody.cpp \
    Box2D/Collision/Shapes/b2ChainShape.cpp \
    Box2D/Collision/Shapes/b2CircleShape.cpp \
    Box2D/Collision/Shapes/b2EdgeShape.cpp \
    Box2D/Collision/Shapes/b2PolygonShape.cpp \
    Box2D/Collision/b2BroadPhase.cpp \
    Box2D/Collision/b2CollideCircle.cpp \
    Box2D/Collision/b2CollideEdge.cpp \
    Box2D/Collision/b2CollidePolygon.cpp \
    Box2D/Collision/b2Collision.cpp \
    Box2D/Collision/b2Distance.cpp \
    Box2D/Collision/b2DynamicTree.cpp \
    Box2D/Collision/b2TimeOfImpact.cpp \
    Box2D/Common/b2BlockAllocator.cpp \
    Box2D/Common/b2Draw.cpp \
    Box2D/Common/b2FreeList.cpp \
    Box2D/Common/b2Math.cpp \
    Box2D/Common/b2Settings.cpp \
    Box2D/Common/b2StackAllocator.cpp \
    Box2D/Common/b2Stat.cpp \
    Box2D/Common/b2Timer.cpp \
    Box2D/Common/b2TrackedBlock.cpp \
    Box2D/Dynamics/Contacts/b2ChainAndCircleContact.cpp \
    Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.cpp \
    Box2D/Dynamics/Contacts/b2CircleContact.cpp \
    Box2D/Dynamics/Contacts/b2Contact.cpp \
    Box2D/Dynamics/Contacts/b2ContactSolver.cpp \
    Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.cpp \
    Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.cpp \
    Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.cpp \
    Box2D/Dynamics/Contacts/b2PolygonContact.cpp \
    Box2D/Dynamics/Joints/b2DistanceJoint.cpp \
    Box2D/Dynamics/Joints/b2FrictionJoint.cpp \
    Box2D/Dynamics/Joints/b2GearJoint.cpp \
    Box2D/Dynamics/Joints/b2Joint.cpp \
    Box2D/Dynamics/Joints/b2MotorJoint.cpp \
    Box2D/Dynamics/Joints/b2MouseJoint.cpp \
    Box2D/Dynamics/Joints/b2PrismaticJoint.cpp \
    Box2D/Dynamics/Joints/b2PulleyJoint.cpp \
    Box2D/Dynamics/Joints/b2RevoluteJoint.cpp \
    Box2D/Dynamics/Joints/b2RopeJoint.cpp \
    Box2D/Dynamics/Joints/b2WeldJoint.cpp \
    Box2D/Dynamics/Joints/b2WheelJoint.cpp \
    Box2D/Dynamics/b2Body.cpp \
    Box2D/Dynamics/b2ContactManager.cpp \
    Box2D/Dynamics/b2Fixture.cpp \
    Box2D/Dynamics/b2Island.cpp \
    Box2D/Dynamics/b2World.cpp \
    Box2D/Dynamics/b2WorldCallbacks.cpp \
    Box2D/Particle/b2Particle.cpp \
    Box2D/Particle/b2ParticleGroup.cpp \
    Box2D/Particle/b2ParticleSystem.cpp \
    Box2D/Particle/b2VoronoiDiagram.cpp \
    Box2D/Rope/b2Rope.cpp \
    SceneMain/physics/WeldJoint.cpp \
    SceneMain/physics/Joint.cpp \
    SceneMain/physics/ParticleSystem.cpp \
    SceneMain/physics/Body.cpp \
    SceneMain/physics/Engine.cpp \
    SceneMain/MyDebugDrawer.cpp \
    SceneMain/physics/DistanceJoint.cpp \
    SceneMain/physics/RevoluteJoint.cpp \
    SceneMain/Manager.cpp

HEADERS += \
	commons.hpp \
	SceneMain/SceneMain.hpp \
	SceneMain/DeferredContainer.hpp \
	SceneMain/DeferredLight.hpp \
	SceneMain/BlurContainer.hpp \
	SceneMain/Player.hpp \
    SceneMain/physics/Collider.hpp \
    SceneMain/physics/CircleCollider.hpp \
    SceneMain/physics/PolygonCollider.hpp \
    SceneMain/GenericBody.hpp \
    Box2D/Collision/Shapes/b2ChainShape.h \
    Box2D/Collision/Shapes/b2CircleShape.h \
    Box2D/Collision/Shapes/b2EdgeShape.h \
    Box2D/Collision/Shapes/b2PolygonShape.h \
    Box2D/Collision/Shapes/b2Shape.h \
    Box2D/Collision/b2BroadPhase.h \
    Box2D/Collision/b2Collision.h \
    Box2D/Collision/b2Distance.h \
    Box2D/Collision/b2DynamicTree.h \
    Box2D/Collision/b2TimeOfImpact.h \
    Box2D/Common/b2BlockAllocator.h \
    Box2D/Common/b2Draw.h \
    Box2D/Common/b2FreeList.h \
    Box2D/Common/b2GrowableStack.h \
    Box2D/Common/b2IntrusiveList.h \
    Box2D/Common/b2Math.h \
    Box2D/Common/b2Settings.h \
    Box2D/Common/b2SlabAllocator.h \
    Box2D/Common/b2StackAllocator.h \
    Box2D/Common/b2Stat.h \
    Box2D/Common/b2Timer.h \
    Box2D/Common/b2TrackedBlock.h \
    Box2D/Dynamics/Contacts/b2ChainAndCircleContact.h \
    Box2D/Dynamics/Contacts/b2ChainAndPolygonContact.h \
    Box2D/Dynamics/Contacts/b2CircleContact.h \
    Box2D/Dynamics/Contacts/b2Contact.h \
    Box2D/Dynamics/Contacts/b2ContactSolver.h \
    Box2D/Dynamics/Contacts/b2EdgeAndCircleContact.h \
    Box2D/Dynamics/Contacts/b2EdgeAndPolygonContact.h \
    Box2D/Dynamics/Contacts/b2PolygonAndCircleContact.h \
    Box2D/Dynamics/Contacts/b2PolygonContact.h \
    Box2D/Dynamics/Joints/b2DistanceJoint.h \
    Box2D/Dynamics/Joints/b2FrictionJoint.h \
    Box2D/Dynamics/Joints/b2GearJoint.h \
    Box2D/Dynamics/Joints/b2Joint.h \
    Box2D/Dynamics/Joints/b2MotorJoint.h \
    Box2D/Dynamics/Joints/b2MouseJoint.h \
    Box2D/Dynamics/Joints/b2PrismaticJoint.h \
    Box2D/Dynamics/Joints/b2PulleyJoint.h \
    Box2D/Dynamics/Joints/b2RevoluteJoint.h \
    Box2D/Dynamics/Joints/b2RopeJoint.h \
    Box2D/Dynamics/Joints/b2WeldJoint.h \
    Box2D/Dynamics/Joints/b2WheelJoint.h \
    Box2D/Dynamics/b2Body.h \
    Box2D/Dynamics/b2ContactManager.h \
    Box2D/Dynamics/b2Fixture.h \
    Box2D/Dynamics/b2Island.h \
    Box2D/Dynamics/b2TimeStep.h \
    Box2D/Dynamics/b2World.h \
    Box2D/Dynamics/b2WorldCallbacks.h \
    Box2D/Particle/b2Particle.h \
    Box2D/Particle/b2ParticleGroup.h \
    Box2D/Particle/b2ParticleSystem.h \
    Box2D/Particle/b2StackQueue.h \
    Box2D/Particle/b2VoronoiDiagram.h \
    Box2D/Rope/b2Rope.h \
    Box2D/Box2D.h \
    SceneMain/physics/Contact.hpp \
    SceneMain/physics/WeldJoint.hpp \
    SceneMain/physics/Joint.hpp \
    SceneMain/physics/ParticleSystem.hpp \
    SceneMain/physics/Body.hpp \
    SceneMain/physics/DebugDrawer.hpp \
    SceneMain/physics/Engine.hpp \
    SceneMain/MyDebugDrawer.hpp \
    SceneMain/physics/Physics.hpp \
    SceneMain/physics/DistanceJoint.hpp \
    SceneMain/physics/RevoluteJoint.hpp \
    SceneMain/Manager.hpp

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



