cmake_minimum_required(VERSION 3.15)
project(SeaConquest CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(PhysFS REQUIRED)

set(MOTOR2D_DIR ${CMAKE_SOURCE_DIR}/Motor2D)

set(SEA_CONQUEST_SOURCES
    ${MOTOR2D_DIR}/FadeToColor.cpp
    ${MOTOR2D_DIR}/Harvester.cpp
    ${MOTOR2D_DIR}/j1AnimationManager.cpp
    ${MOTOR2D_DIR}/j1App.cpp
    ${MOTOR2D_DIR}/j1AssetManager.cpp
    ${MOTOR2D_DIR}/j1Audio.cpp
    ${MOTOR2D_DIR}/j1Balloon.cpp
    ${MOTOR2D_DIR}/j1Boat.cpp
    ${MOTOR2D_DIR}/j1BoatHouse.cpp
    ${MOTOR2D_DIR}/j1Button.cpp
    ${MOTOR2D_DIR}/j1Carrier.cpp
    ${MOTOR2D_DIR}/j1Color.cpp
    ${MOTOR2D_DIR}/j1DialogSystem.cpp
    ${MOTOR2D_DIR}/j1Element.cpp
    ${MOTOR2D_DIR}/j1Entity.cpp
    ${MOTOR2D_DIR}/j1EntityManager.cpp
    ${MOTOR2D_DIR}/j1Explanation.cpp
    ${MOTOR2D_DIR}/j1Fog.cpp
    ${MOTOR2D_DIR}/j1Font.cpp
    ${MOTOR2D_DIR}/j1Fonts.cpp
    ${MOTOR2D_DIR}/j1GUI.cpp
    ${MOTOR2D_DIR}/j1Image.cpp
    ${MOTOR2D_DIR}/j1InGameUI.cpp
    ${MOTOR2D_DIR}/j1InputBox.cpp
    ${MOTOR2D_DIR}/j1Input.cpp
    ${MOTOR2D_DIR}/j1Label.cpp
    ${MOTOR2D_DIR}/j1LogoScene.cpp
    ${MOTOR2D_DIR}/j1Main.cpp
    ${MOTOR2D_DIR}/j1MainMenuUI.cpp
    ${MOTOR2D_DIR}/j1Map.cpp
    ${MOTOR2D_DIR}/j1Minimap.cpp
    ${MOTOR2D_DIR}/j1ParticleManager.cpp
    ${MOTOR2D_DIR}/j1Pathfinding.cpp
    ${MOTOR2D_DIR}/j1PerfTimer.cpp
    ${MOTOR2D_DIR}/j1Player.cpp
    ${MOTOR2D_DIR}/j1QuestManager.cpp
    ${MOTOR2D_DIR}/j1Render.cpp
    ${MOTOR2D_DIR}/j1Resource.cpp
    ${MOTOR2D_DIR}/j1Scene2.cpp
    ${MOTOR2D_DIR}/j1Scene.cpp
    ${MOTOR2D_DIR}/j1SceneManager.cpp
    ${MOTOR2D_DIR}/j1ScrollBar.cpp
    ${MOTOR2D_DIR}/j1Storage.cpp
    ${MOTOR2D_DIR}/j1Structure.cpp
    ${MOTOR2D_DIR}/j1Textures.cpp
    ${MOTOR2D_DIR}/j1Timer.cpp
    ${MOTOR2D_DIR}/j1TownHall.cpp
    ${MOTOR2D_DIR}/j1TransitionManager.cpp
    ${MOTOR2D_DIR}/j1Transitions.cpp
    ${MOTOR2D_DIR}/j1Unit.cpp
    ${MOTOR2D_DIR}/j1Window.cpp
    ${MOTOR2D_DIR}/Lines.cpp
    ${MOTOR2D_DIR}/p2Log.cpp
    ${MOTOR2D_DIR}/Particle.cpp
    ${MOTOR2D_DIR}/ParticleSystem.cpp
    ${MOTOR2D_DIR}/PugiXml/src/pugixml.cpp
    ${MOTOR2D_DIR}/random.cpp
    ${MOTOR2D_DIR}/Squares.cpp
    ${MOTOR2D_DIR}/Turret.cpp
    ${MOTOR2D_DIR}/Wipe.cpp
)

set(SEA_CONQUEST_HEADERS
    ${MOTOR2D_DIR}/j1AnimationManager.h
    ${MOTOR2D_DIR}/Color.h
    ${MOTOR2D_DIR}/FadeToColor.h
    ${MOTOR2D_DIR}/j1AssetManager.h
    ${MOTOR2D_DIR}/j1DialogSystem.h
    ${MOTOR2D_DIR}/j1Explanation.h
    ${MOTOR2D_DIR}/j1Fog.h
    ${MOTOR2D_DIR}/j1LogoScene.h
    ${MOTOR2D_DIR}/j1Color.h
    ${MOTOR2D_DIR}/j1Entities.h
    ${MOTOR2D_DIR}/j1EntityManager.h
    ${MOTOR2D_DIR}/j1Font.h
    ${MOTOR2D_DIR}/j1Fonts.h
    ${MOTOR2D_DIR}/j1GUI.h
    ${MOTOR2D_DIR}/j1GUIElements.h
    ${MOTOR2D_DIR}/j1InGameUI.h
    ${MOTOR2D_DIR}/j1MainMenuUI.h
    ${MOTOR2D_DIR}/j1Map.h
    ${MOTOR2D_DIR}/j1Minimap.h
    ${MOTOR2D_DIR}/j1Pathfinding.h
    ${MOTOR2D_DIR}/j1PerfTimer.h
    ${MOTOR2D_DIR}/j1Player.h
    ${MOTOR2D_DIR}/j1QuestManager.h
    ${MOTOR2D_DIR}/j1Scene.h
    ${MOTOR2D_DIR}/j1Scene2.h
    ${MOTOR2D_DIR}/j1SceneManager.h
    ${MOTOR2D_DIR}/j1Timer.h
    ${MOTOR2D_DIR}/j1TransitionManager.h
    ${MOTOR2D_DIR}/j1Transitions.h
    ${MOTOR2D_DIR}/Lines.h
    ${MOTOR2D_DIR}/p2DynArray.h
    ${MOTOR2D_DIR}/j1Audio.h
    ${MOTOR2D_DIR}/j1Input.h
    ${MOTOR2D_DIR}/p2Log.h
    ${MOTOR2D_DIR}/j1App.h
    ${MOTOR2D_DIR}/p2Defs.h
    ${MOTOR2D_DIR}/p2List.h
    ${MOTOR2D_DIR}/j1Module.h
    ${MOTOR2D_DIR}/p2Point.h
    ${MOTOR2D_DIR}/j1Render.h
    ${MOTOR2D_DIR}/j1Textures.h
    ${MOTOR2D_DIR}/j1Window.h
    ${MOTOR2D_DIR}/p2SString.h
    ${MOTOR2D_DIR}/Particle.h
    ${MOTOR2D_DIR}/j1ParticleManager.h
    ${MOTOR2D_DIR}/ParticleSystem.h
    ${MOTOR2D_DIR}/PugiXml/src/pugiconfig.hpp
    ${MOTOR2D_DIR}/PugiXml/src/pugixml.hpp
    ${MOTOR2D_DIR}/random.h
    ${MOTOR2D_DIR}/Squares.h
    ${MOTOR2D_DIR}/Wipe.h
)

add_executable(SeaConquest ${SEA_CONQUEST_SOURCES} ${SEA_CONQUEST_HEADERS})

target_include_directories(SeaConquest PRIVATE
    ${MOTOR2D_DIR}
)

target_link_libraries(SeaConquest PRIVATE
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
    physfs
)

if(WIN32)
    target_link_libraries(SeaConquest PRIVATE SDL2::SDL2main)
endif()
