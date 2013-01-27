AC_INIT([SimpleGame], [0.1])
AM_INIT_AUTOMAKE([foreign subdir-objects])
AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_HEADERS([config.h])     # not even really needed
AC_PROG_CXX                        # or AC_PROG_CXX

# Check if the vectormath headers exist
AC_CHECK_HEADER([vectormath/cpp/vectormath_aos.h])

# Find the following libraries on the system
PKG_CHECK_MODULES([gl], [gl])
PKG_CHECK_MODULES([glew], [glew])
PKG_CHECK_MODULES([sdl], [sdl])

AC_CONFIG_FILES([Makefile
	src/Makefile])
AC_OUTPUT
