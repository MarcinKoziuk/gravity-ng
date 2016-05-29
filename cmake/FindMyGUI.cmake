find_path(MYGUI_INCLUDE_DIR MYGUI/MyGUI.h)
find_library(MYGUI_LIBRARY NAMES MyGUIEngineStatic MyGUIEngineStatic_d PATH_SUFFIXES Debug)
find_library(MYGUI_OPENGLSYSTEM_LIBRARY NAMES MyGUI.OpenGLPlatform_d MyGUI.OpenGLPlatform PATH_SUFFIXES Debug Release)


INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(MYGUI DEFAULT_MSG MYGUI_LIBRARY MYGUI_INCLUDE_DIR)

MARK_AS_ADVANCED(
  MYGUI_INCLUDE_DIR
  MYGUI_LIBRARY
)