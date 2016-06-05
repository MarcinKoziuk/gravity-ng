find_path(SDL_gpu_INCLUDE_DIR SDL/SDL_gpu.h)
find_library(SDL_gpu_LIBRARY NAMES SDL2_gpu)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL_gpu DEFAULT_MSG SDL_gpu_LIBRARY SDL_gpu_INCLUDE_DIR)

MARK_AS_ADVANCED(
  SDL_gpu_INCLUDE_DIR
  SDL_gpu_LIBRARY
)