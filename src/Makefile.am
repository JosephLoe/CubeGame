bin_PROGRAMS = simplegame
simplegame_SOURCES = Main.cpp GameAsset.cpp Md2Asset.cpp TriangularPyramidAsset.cpp BallisticInterpolator.cpp Camera.cpp BoundingBox.cpp

AM_CPPFLAGS =  -I../include/ -std=c++0x
AM_CPPFLAGS += $(gl_CFLAGS) $(glew_CFLAGS) $(sdl_CFLAGS)
LIBS += $(gl_LIBS) $(glew_LIBS) $(sdl_LIBS)