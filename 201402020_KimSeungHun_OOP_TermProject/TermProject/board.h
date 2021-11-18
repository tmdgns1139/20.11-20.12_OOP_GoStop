#ifndef _BOARD_H_
#define _BOARD_H_

#include <list>
#include <stack>
#include <string>
#include <vector>

//#include "card.h"
#include "player.h"

class Board {
 public:
  // dack에 대한 정보를 문자열로 반환한다.
  // (ex) ===============
  //      | Deck (20개) |
  //      ---------------
  std::string ToStringForDeck();

  // 바닥에 깔린 카드들을 문자열로 반환한다.
  // (ex) | 8(gwang_normal) | 1(p_normal) | 3(p_noraml) |
  //      | 7(animal_normal) |
  //      ===============================================
  std::string ToStringForFloor();

  // dack에서 카드를 하나 꺼낸다.
  Card* DrawCardFromDeck();

  // 상대로부터 피를 하나 빼앗아온다.
  bool StealPIfPossible(Player* opponent_player,
                        std::list<Card*>* from_board_to_player);

  // Board가 player에게 보내는 카드 리스트를 반환한다.
  std::list<Card*>* CardsToPlayer(Card* card_from_player,
                                Player* opponent_player);

  // dack이 비어있는지 확인한다.
  bool DeckIsEmpty();

  // card와 매칭되는 카드들이 floor에 있는지 확인하여 vector로 모두 반환한다.
  std::vector<Card*> MatchedCardsWith(Card* card);

  // from에 있는 카드들을 경우에 따라 to에 옮겨 담는다.
  void InsertCardsInto(Player* opponent_player, const std::vector<Card*>& from,
                       std::list<Card*>* to);

  // Getters
  std::stack<Card*>* deck();

  // Setters
  void set_floor(std::list<Card*>* new_floor);
  void set_deck(std::stack<Card*>* new_deck);

 private:
  std::stack<Card*>* deck_;
  std::list<Card*>* floor_;
};

#endif  // _BOARD_H_
