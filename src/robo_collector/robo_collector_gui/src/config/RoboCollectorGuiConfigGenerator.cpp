//Corresponding header
#include "robo_collector_gui/config/RoboCollectorGuiConfigGenerator.h"

//C system headers

//C++ system headers

//Other libraries headers
#include <rclcpp/utilities.hpp>
#include <ament_index_cpp/get_package_share_directory.hpp>
#include "robo_common/defines/RoboCommonDefines.h"
#include "robo_common/helpers/ConfigFileLoader.h"
#include "resource_utils/common/ResourceFileHeader.h"
#include "utils/ErrorCode.h"
#include "utils/Log.h"

//Own components headers
#include "robo_collector_gui/config/RoboCollectorGuiConfig.h"
#include "robo_collector_gui/defines/RoboCollectorGuiDefines.h"
#include "generated/RoboCollectorGuiResources.h"

namespace {
//TODO parse the params from config
constexpr auto PROJECT_FOLDER_NAME = "robo_collector_gui";

//screen
constexpr auto WINDOW_X = 72;
constexpr auto WINDOW_Y = 27;
constexpr auto WINDOW_WIDTH = 1848;
constexpr auto WINDOW_HEIGHT = 1053;

//field
constexpr auto TILE_WIDTH_HEIGHT = 160;
constexpr auto ROBOT_FIELD_MARKERS = RobotFieldMarkers::ENABLED;

//misc
constexpr auto TOTAL_GAME_SECONDS = 180;
constexpr auto LOCAL_CONTROLLER_MODE = LocalControllerMode::DISABLED;
constexpr auto TARGET_WIN_COINS = 30;

enum TimerId {
  ROBOTS_MOVE_ANIM_TIMER_ID_START,
  //reserved
  ROBOTS_MOVE_ANIM_TIMER_ID_END = ROBOTS_MOVE_ANIM_TIMER_ID_START
      + Defines::ENEMIES_CTN,

  ROBOTS_WALL_COLLISION_ANIM_TIMER_ID_START,
  //reserved
  ROBOTS_WALL_COLLISION_ANIM_TIMER_ID_END = ROBOTS_WALL_COLLISION_ANIM_TIMER_ID_START
      + Defines::ENEMIES_CTN,

  ROBOTS_ROBOT_COLLISION_ANIM_TIMER_ID_START,
  //reserved
  ROBOTS_ROBOT_COLLISION_ANIM_TIMER_ID_END = ROBOTS_ROBOT_COLLISION_ANIM_TIMER_ID_START
      + Defines::ENEMIES_CTN,

  ROBOTS_ROBOT_DAMAGE_ANIM_TIMER_ID_START,
  //reserved
  ROBOTS_ROBOT_DAMAGE_ANIM_TIMER_ID_END = ROBOTS_ROBOT_DAMAGE_ANIM_TIMER_ID_START
      + Defines::ENEMIES_CTN,

  COIN_ROTATE_ANIM_TIMER_ID_START,
  //reserved
  COIN_ROTATE_ANIM_TIMER_ID_END = COIN_ROTATE_ANIM_TIMER_ID_START
      + Defines::COINS_CTN,

  COIN_COLLECT_ANIM_TIMER_ID_START,
  //reserved
  COIN_COLLECT_ANIM_TIMER_ID_END = COIN_COLLECT_ANIM_TIMER_ID_START
      + Defines::COINS_CTN,

  COIN_RESPAWN_ANIM_TIMER_ID_START,
  //reserved
  COIN_RESPAWN_ANIM_TIMER_ID_END = COIN_RESPAWN_ANIM_TIMER_ID_START
      + Defines::COINS_CTN,

  COIN_PANEL_INCR_TIMER_ID,
  COIN_PANEL_DECR_TIMER_ID,
  TIME_PANEL_CLOCK_TIMER_ID,
  TIME_PANEL_BLINK_TIMER_ID,
  HEALTH_PANEL_REDUCE_INDICATOR_TIMER_ID
};

RobotBaseConfig generateRobotBaseConfig() {
  RobotBaseConfig cfg;

  cfg.playerRsrcId = RoboCollectorGuiResources::PLAYER_ROBOT;
  cfg.enemiesRsrcId = RoboCollectorGuiResources::ENEMY_ROBOTS;
  cfg.damageMarkerRsrcId = RoboCollectorGuiResources::DAMAGE_MARKER;
  cfg.moveAnimStartTimerId = ROBOTS_MOVE_ANIM_TIMER_ID_START;
  cfg.wallCollisionAnimStartTimerId = ROBOTS_WALL_COLLISION_ANIM_TIMER_ID_START;
  cfg.robotCollisionAnimStartTimerId =
      ROBOTS_ROBOT_COLLISION_ANIM_TIMER_ID_START;
  cfg.robotDamageAnimStartTimerId = ROBOTS_ROBOT_DAMAGE_ANIM_TIMER_ID_START;
  cfg.robotFieldMarkers = ROBOT_FIELD_MARKERS;

  return cfg;
}

RoboCollectorUiControllerBaseConfig generateRoboCollectorUiControllerConfig() {
  RoboCollectorUiControllerBaseConfig cfg;

  cfg.moveButtonsRsrcIds = { RoboCollectorGuiResources::UP_BUTTON,
      RoboCollectorGuiResources::LEFT_BUTTON,
      RoboCollectorGuiResources::RIGHT_BUTTON };
  cfg.moveButtonInfoTextFontId = RoboCollectorGuiResources::VINQUE_RG_30;
  cfg.horDelimiterRsrcId = RoboCollectorGuiResources::HOR_DELIMITER;
  cfg.vertDelimiterRsrcId = RoboCollectorGuiResources::VERT_DELIMITER;
  cfg.helpButtonRsrcId = RoboCollectorGuiResources::HELP_BUTTON;
  cfg.settingsButtonRsrcId = RoboCollectorGuiResources::SETTINGS_BUTTON;
  cfg.localControllerMode = LOCAL_CONTROLLER_MODE;

  return cfg;
}

CoinHandlerConfig generateCoinHandlerConfig() {
  CoinHandlerConfig cfg;

  cfg.animRsrcIds = { RoboCollectorGuiResources::COIN_ANIM_GOLD,
      RoboCollectorGuiResources::COIN_ANIM_SILVER,
      RoboCollectorGuiResources::COIN_ANIM_BRONZE };
  cfg.fieldMarkers = { 'g', 's', 'b' //gold, silver, bronze
      };
  cfg.maxCoins = Defines::COINS_CTN;
  cfg.targetWinCoins = TARGET_WIN_COINS;
  cfg.rotateAnimFirstTimerId = COIN_ROTATE_ANIM_TIMER_ID_START;
  cfg.collectAnimFirstTimerId = COIN_COLLECT_ANIM_TIMER_ID_START;
  cfg.respawnAnimFirstTimerId = COIN_RESPAWN_ANIM_TIMER_ID_START;
  cfg.fieldEmptyMarker = RoboCommonDefines::EMPTY_TILE_MARKER;

  return cfg;
}

PanelHandlerConfig generatePanelHandlerConfig() {
  PanelHandlerConfig cfg;

  auto &healthPanelCfg = cfg.healthPanelCfg;
  healthPanelCfg.rsrcId = RoboCollectorGuiResources::HEALTH_PANEL;
  healthPanelCfg.indicatorRsrcId = RoboCollectorGuiResources::HEALTH_INDICATOR;
  healthPanelCfg.indicatorFontId = RoboCollectorGuiResources::VINQUE_RG_30;
  healthPanelCfg.indicatorReduceTimerId =
      HEALTH_PANEL_REDUCE_INDICATOR_TIMER_ID;

  auto &coinPanelCfg = cfg.coinPanelCfg;
  coinPanelCfg.targetNumber = TARGET_WIN_COINS;
  coinPanelCfg.rsrcId = RoboCollectorGuiResources::COIN_PANEL;
  coinPanelCfg.fontId = RoboCollectorGuiResources::VINQUE_RG_75;
  coinPanelCfg.incrTimerId = COIN_PANEL_INCR_TIMER_ID;
  coinPanelCfg.decrTimerId = COIN_PANEL_DECR_TIMER_ID;

  auto &timePanelCfg = cfg.timePanelCfg;
  timePanelCfg.rsrcId = RoboCollectorGuiResources::TIME_PANEL;
  timePanelCfg.fontId = RoboCollectorGuiResources::VINQUE_RG_75;
  timePanelCfg.clockTimerId = TIME_PANEL_CLOCK_TIMER_ID;
  timePanelCfg.blinkTimerId = TIME_PANEL_BLINK_TIMER_ID;
  timePanelCfg.totalSeconds = TOTAL_GAME_SECONDS;

  return cfg;
}

FieldConfig generateFieldConfig() {
  FieldConfig cfg;

  const auto projectInstallPrefix =
      ament_index_cpp::get_package_share_directory(PROJECT_FOLDER_NAME);
  const auto levelId = 1;
  cfg.description.data =
      ConfigFileLoader::readFieldData(projectInstallPrefix, levelId);

  cfg.description.rows = cfg.description.data.size();
  if (!cfg.description.data.empty()) {
    cfg.description.cols = static_cast<int32_t>(cfg.description.data[0].size());
  }
  cfg.description.tileWidth = TILE_WIDTH_HEIGHT;
  cfg.description.tileHeight = TILE_WIDTH_HEIGHT;
  cfg.description.emptyDataMarker = RoboCommonDefines::EMPTY_TILE_MARKER;
  cfg.description.hardObstacleMarker = RoboCommonDefines::HARD_OBSTACLE_MARKER;
  cfg.tileRsrcId = RoboCollectorGuiResources::MAP_TILE;
  cfg.debugFontRsrcId = RoboCollectorGuiResources::VINQUE_RG_30;

  return cfg;
}

EngineConfig generateEngineConfig() {
  const auto projectInstallPrefix =
      ament_index_cpp::get_package_share_directory(PROJECT_FOLDER_NAME);
  auto cfg = getDefaultEngineConfig(projectInstallPrefix);

  auto &windowCfg = cfg.managerHandlerCfg.drawMgrCfg.monitorWindowConfig;
  windowCfg.name = PROJECT_FOLDER_NAME;
  windowCfg.iconPath.append(projectInstallPrefix).append("/").append(
      ResourceFileHeader::getResourcesFolderName()).append(
      "/p/entities/player_robot.png");
  windowCfg.pos = Point(WINDOW_X, WINDOW_Y);
  windowCfg.width = WINDOW_WIDTH;
  windowCfg.height = WINDOW_HEIGHT;
  windowCfg.displayMode = WindowDisplayMode::WINDOWED;
  windowCfg.borderMode = WindowBorderMode::BORDERLESS;

  cfg.debugConsoleRsrcId = RoboCollectorGuiResources::VINQUE_RG_30;

  return cfg;
}

RoboCollectorGuiConfig generateGameConfig() {
  RoboCollectorGuiConfig cfg;
  auto &layoutCfg = cfg.layoutCfg;
  layoutCfg.panelHandlerCfg = generatePanelHandlerConfig();
  layoutCfg.coinHandlerCfg = generateCoinHandlerConfig();
  layoutCfg.controllerCfg = generateRoboCollectorUiControllerConfig();

  auto &commonLayoutCfg = layoutCfg.commonLayoutCfg;
  commonLayoutCfg.fieldCfg = generateFieldConfig();
  commonLayoutCfg.robotBaseCfg = generateRobotBaseConfig();
  commonLayoutCfg.mapRsrcId = RoboCollectorGuiResources::MAP;
  commonLayoutCfg.playerFieldMarker = RoboCommonDefines::PLAYER_MARKER;
  commonLayoutCfg.enemyFieldMarker = RoboCommonDefines::ENEMY_MARKER;

  return cfg;
}

} //end anonymous namespace

std::vector<DependencyDescription> RoboCollectorGuiConfigGenerator::generateDependencies(
    int32_t argc, char **args) {
  std::vector<DependencyDescription> dependecies = getDefaultEngineDependencies(
      argc, args);

  const LoadDependencyCb ros2Loader = [argc, args]() {
    rclcpp::init(argc, args);
    return SUCCESS;
  };
  const UnloadDependencyCb ros2Unloader = []() {
    //shutdown the global context only if it hasn't
    //for example: ROS2 signal handlers do that automatically
    if (rclcpp::ok()) {
      const bool success = rclcpp::shutdown();
      if (!success) {
        LOGERR("Error, global context was already shutdowned");
      }
    }
  };

  dependecies.push_back( { "ROS2", ros2Loader, ros2Unloader });

  return dependecies;
}

ApplicationConfig RoboCollectorGuiConfigGenerator::generateConfig() {
  ApplicationConfig cfg;
  cfg.engineCfg = generateEngineConfig();
  cfg.gameCfg = generateGameConfig();
  return cfg;
}

