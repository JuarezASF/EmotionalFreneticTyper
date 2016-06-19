project(game02)

add_executable(game02 main.cpp Game.cpp GameException.cpp StageState.cpp Sprite.cpp Vec2.cpp  Rect.cpp  GameObject.cpp StateInput.cpp TileSet.cpp  GameException.cpp  TileMap.cpp CommandLineParser.cpp GameConfig.cpp Resources.cpp InputManager.cpp Logger.cpp Camera.cpp Timer.cpp Animation.cpp State.cpp TitleState.cpp TitleState.h Music.cpp Music.h Sound.cpp Sound.h Text.cpp Text.h EndState.cpp StateData.cpp PlayableEmotion.cpp PlayableEmotion.h TyperInput.cpp TyperInput.h CollidableAABBGameObject.cpp CollidableAABBGameObject.h jasf_math.cpp Collidable.cpp Collidable.h AxisAlignedBoundingBox.cpp AxisAlignedBoundingBox.h ForceField.cpp ForceField.h KillingRectangle.cpp KillingRectangle.h Panel.cpp Panel.h CollidableCircle.cpp CollidableCircle.h StageBuilderStage.cpp SettingsLoad.cpp)

include_directories(include ${SDL2_IMAGE_INCLUDE_DIR})

target_link_libraries(game02 ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDLMIXER_LIBRARY} ${SDL2_TTF_LIBRARY} ${SDL2GFX_LIBRARY})

install(TARGETS game02 RUNTIME DESTINATION ${BIN_DIR})
