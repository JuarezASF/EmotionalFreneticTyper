#include <iostream>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "GameException.h"
#include "Game.h"
#include "CommandLineParser.h"
#include "GameException.h"
#include "GameConfig.h"
#include "Logger.h"
#include "StageState.h"
#include "TitleState.h"

using namespace std;

int main(int argc, char **argv) {
    Logger::log("Game Starting...");

    CommandLineParser clp;

    clp.addBoolArg("debug", false, false);
    clp.addBoolArg("printHitBox", false, false);

    clp.parseCommandLine(argc, argv);

    GameConfig::setDebugMode(clp.getBoolVal("debug"));
    GameConfig::setPrintHitBot(clp.getBoolVal("printHitBox"));

    try {

        Game g = Game("Juarez Aires 11/0032829", 1024, 600);

        g.push(new TitleState());
        g.run();

    }catch(GameException e){
        Logger::log("Error!");
        Logger::log(e.what());
    }catch(exception e){
        Logger::log("Error!");
        Logger::log(e.what());
    }


    Logger::log("Game Over...");
    return 0;
}
