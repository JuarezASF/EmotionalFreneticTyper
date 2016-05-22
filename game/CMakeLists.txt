project(game01)

add_executable(game01 main.cpp Game.cpp GameException.cpp StageState.cpp Sprite.cpp Vec2.cpp  Rect.cpp  GameObject.cpp StateInput.cpp TileSet.cpp  GameException.cpp  TileMap.cpp CommandLineParser.cpp GameConfig.cpp Resources.cpp InputManager.cpp Logger.cpp Camera.cpp Alien.cpp Minion.cpp Bullet.cpp Penguins.cpp Timer.cpp Animation.cpp State.cpp TitleState.cpp TitleState.h Music.cpp Music.h Sound.cpp Sound.h Text.cpp Text.h EndState.cpp StateData.cpp)

include_directories(include ${SDL2_IMAGE_INCLUDE_DIR})

target_link_libraries(game01 ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDLMIXER_LIBRARY} ${SDL2_TTF_LIBRARY})

add_library(Util Util.cpp Util.h)

add_library(Vec2 Vec2.cpp Vec2.h)

add_library(Rect Rect.cpp Rect.h)

add_library(ArgParse CommandLineParser.cpp CommandLineParser.h EndState.cpp EndState.h StateData.cpp StateData.h)

install(TARGETS game01 RUNTIME DESTINATION ${BIN_DIR})
