#ifndef _JUDGE_H_
#define _JUDGE_H_

#include <stack>
#include <list>
#include <vector>

//#include "card.h"
#include "player.h"

class Judge {
 public:
  // Constructor
  Judge();

  // Destructor
  ~Judge();


  // 10장의 카드를 player에게 준다.
  std::vector<Card*>* CardsForPlayerFromInitDeck(std::stack<Card*>* a_deck);

  // 8장의 카드를 board의 바닥에 깐다.
  std::list<Card*>* CardsForBoardFloor(std::stack<Card*>* a_deck);
  
  // 초기 덱(deck)을 생성한다.
  std::stack<Card*>* InitDeck();

  // 덱을 삭제한다.
  void ClearDeck(std::stack<Card*>* a_deck);

  // 바닥(floor_)에 있는 카드와 바닥을 삭제한다.
  void ClearFloor(std::list<Card*>* a_floor);

  // 플레이어의 점수를 계산한다.
  int CalcScore(Player* a_player, Player* opponent_player);

  // 점수가 7점 이상이면 게임을 종료한다.
  bool IsGameOver(int a_score);

  // Getters & Setters
  bool is_game_over();
  void set_is_game_over(bool new_is_game_over);


 private:
  bool is_game_over_;

  // 게임에 쓸 카드 48장을 만든다.
  std::vector<Card*>* MakeAllCards();

  // 만들어진 48장의 카드를 섞는다.
  std::stack<Card*>* Suffle(std::vector<Card*>* all_cards);

  // 광에 대한 점수를 계산한다.
  int CalcScoreForGwang(Player* a_player);
  // 동물에 대한 점수를 계산한다.
  int CalcScoreForAnimal(Player* a_player, bool& is_mung);
  // 단에 대한 점수를 계산한다.
  int CalcScoreForDan(Player* a_player);
  // 피에 대한 점수를 계산한다.
  int CalcScoreForP(Player* a_player);
};

#endif  // !_JUDGE_H_
