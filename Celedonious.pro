
TEMPLATE = subdirs
# Needed to ensure that things are built right, which you have to do yourself :(
CONFIG += ordered

# All the projects in your application are sub-projects of your solution
SUBDIRS = VBE \
          VBE-Scenegraph \
          VBE-Physics2D \
          game


# Use .depends to specify that a project depends on another.
VBE-Scenegraph.depends = VBE
VBE-Physics2D.depends = VBE VBE-Scenegraph
game.depends = VBE VBE-Scenegraph VBE-Physics2D

OTHER_FILES += \
        common.pri
