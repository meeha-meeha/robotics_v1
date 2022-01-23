#ifndef ROBO_COLLECTOR_GUI_ROBOCOLLECTORGUI_H_
#define ROBO_COLLECTOR_GUI_ROBOCOLLECTORGUI_H_

//C system headers

//C++ system headers
#include <cstdint>
#include <array>

//Other libraries headers
#include "game_engine/Game.h"

//Own components headers
#include "robo_collector_gui/field/Field.h"
#include "robo_collector_gui/entities/robot/Robot.h"
#include "robo_collector_gui/panels/Panel.h"
#include "robo_collector_gui/entities/coin/CoinHandler.h"
#include "robo_collector_gui/controller/RoboCollectorController.h"

//Forward declarations
class InputEvent;
struct RoboCollectorGuiConfig;

class RoboCollectorGui final : public Game {
public:
  int32_t init(const std::any& cfg) override;
  void deinit() override;

  void draw() const override;
  void handleEvent(const InputEvent &e) override;

private:
  int32_t initRobots(const RoboCollectorGuiConfig& cfg);
  int32_t initCoinHandler(const RoboCollectorGuiConfig& cfg);
  int32_t initController(const RoboCollectorGuiConfig& cfg);

  Image _map;
  Panel _panel;
  Field _field;
  CoinHandler _coinHandler;
  RoboCollectorController _controller;
  std::array<Robot, Defines::ROBOTS_CTN> _robots;
};

#endif /* ROBO_COLLECTOR_GUI_ROBOCOLLECTORGUI_H_ */
