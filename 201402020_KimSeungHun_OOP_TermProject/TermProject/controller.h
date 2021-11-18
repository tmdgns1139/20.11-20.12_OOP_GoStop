#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

//#include "player.h"
#include "board.h"
#include "judge.h"

class Controller {
 public:
  // Constructor
  Controller();

  // Destructor
  ~Controller();

  // a_player의 차례이다.
  void PlayOf(Player* a_player, Player* opponent_player);
  
  // 1. 카드 만들기 (48장)
  // 2. 카드를 플레이어에게 10장씩 나누어주기.
  // 3. 카드 8장을 판의 바닥에 깔아두기.
  void InitSetting();

  // 게임을 한판 진행한다.
  void PlayGame();

  void Run();

 private:
  Player player_A_;
  Player player_B_;
  Board board_;
  Judge judge_;
};

#endif  // _CONTROLLER_H_
