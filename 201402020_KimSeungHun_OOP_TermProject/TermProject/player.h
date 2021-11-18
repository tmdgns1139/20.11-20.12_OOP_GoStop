#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <vector>
#include <string>
#include <list>

#include "card.h"
#include "state.h"
#include "pocket.h"

class Player {
 public:
  // Constructor
  Player();

  // Destructor
  ~Player();
  
  // 주머니에 카드를 넣는다.
  void AddCard(Card* a_card);

  // 주머니에서 피카드를 하나 꺼낸다.
  Card* GetPIfPossible();

  // 고를 선언하고 현재 상태를 갱신한다.
  void DeclareGo(int score_when_declare_go);

  // 현재 플레이어가 낼 수 있는 카드를 문자열로 얻는다.
  const std::string ToStringForAvailableCards();

  // 주머니에 있는 카드들을 정보를 문자열로 반환한다.
  const std::string ToStringForPockets();

  // 플레이어가 낼 카드를 선택하여 낸다.
  // 심판은 플레이어가 한 턴에 한번만 카드를 내도록 통제한다.
  Card* DropCard(int selected_card_order);

  // 플레이어의 주머니를 비운다.
  void CleanPocketsAndResetScore();

  // Getters
  std::vector<Card*>* available_cards();
  Pocket* pockets_for_cards();
  State* current_state();

  // Setters
  void set_avilable_cards(std::vector<Card*>* new_available_cards);
  

 private:
  std::vector<Card*>* available_cards_;
  Pocket* pockets_for_cards_;
  State* current_state_;
};

#endif  // _PLAYER_H_
