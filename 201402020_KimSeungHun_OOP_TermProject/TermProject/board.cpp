#include "board.h"

#include <list>
#include <string>
#include <vector>

#include "view.h"

// dack에 대한 정보를 문자열로 반환한다.
// (ex) =================
//      | Deck ( 20장 ) |
//      -----------------
std::string Board::ToStringForDeck() {
  std::string answer = " =================\n ";
  answer += "| Deck ( " + std::to_string(deck_->size()) + "장 ) |\n";
  answer += " -----------------";
  return answer;
}

// 바닥에 깔린 카드들을 문자열로 반환한다.
// (ex) | 8(gwang_normal) | 1(p_normal) | 3(p_noraml) |
//      | 7(animal_normal) |
//      ===============================================
std::string Board::ToStringForFloor() {
  if (floor_->empty()) {
    std::string answer = "!! 현재 바닥에 카드가 없습니다. !!\n";
    answer +=
        "======================================================================"
        "==";
    return answer;

  } else {
    std::string answer = "";
    std::list<Card*>::iterator it_for_floor = floor_->begin();
    int count = 0;

    for (; it_for_floor != floor_->end(); it_for_floor++) {
      answer += "| " + (*it_for_floor)->ToString() + " ";
      count++;
      if (count % 5 == 0) {
        answer += "|\n";
      }
    }
    if (count % 5 != 0) {
      answer += "|\n";
    }
    answer +=
        "======================================================================"
        "==";
    return answer;
  }
}

// card와 매칭되는 카드들이 floor에 있는지 확인하여 vector로 모두 반환한다.
std::vector<Card*> Board::MatchedCardsWith(Card* card) {
  std::vector<Card*> answer = std::vector<Card*>();

  std::vector<std::list<Card*>::iterator> iterators_will_be_deleted =
      std::vector<std::list<Card*>::iterator>();
  std::list<Card*>::iterator it;
  for (it = floor_->begin(); it != floor_->end(); it++) {
    if ((*it)->month() == card->month()) {
      answer.push_back(*it);
      iterators_will_be_deleted.push_back(it);
    }
  }

  for (std::list<Card*>::iterator it : iterators_will_be_deleted) {
    floor_->erase(it);
  }

  return answer;
}

// deck에서 카드를 하나 꺼낸다.
Card* Board::DrawCardFromDeck() {
  if (deck_->empty()) {
    return nullptr; // delete Board::CardsToPlayer
  } else {
    Card* answer = deck_->top();
    deck_->pop();
    return answer;
  }
}

// 상대방으로부터 피를 하나 빼았아 온다.
bool Board::StealPIfPossible(Player* opponent_player,
                             std::list<Card*>* from_board_to_player) {
  Card* p_from_opponent_player = opponent_player->GetPIfPossible();
  if (p_from_opponent_player) {
    from_board_to_player->push_back(p_from_opponent_player);
    View::PrintStringWithLine("!! [Board says] 상대방에게 빼앗은 피: " +
                              p_from_opponent_player->ToString());
    return true;

  } else {
    View::PrintStringWithLine(
        "!! [Board says] 상대방에게서 빼앗을 피가 없습니다.");
    return false;
  }
}

// card_from_player을 받은 Board가
// player에게 보내는 카드 리스트를 반환한다.
std::list<Card*>* Board::CardsToPlayer(Card* card_from_player,
                                       Player* opponent_player) {
  std::list<Card*>* answer = new std::list<Card*>();

  Card* card_from_deck = DrawCardFromDeck();
  if (!card_from_deck) return nullptr;
  
  View::PrintStringWithLine(">> [Board says] 플레이어가 낸 카드: " +
                            card_from_player->ToString());
  View::PrintStringWithLine(">> [Board says] 덱에서 나온 카드: " +
                            card_from_deck->ToString());

  if (card_from_player->month() == card_from_deck->month()) {
    std::vector<Card*> matched_cards = MatchedCardsWith(card_from_player);

    if (matched_cards.size() == 2) {
      // 바닥에 2개의 카드가 겹침 ... 따닥
      View::PrintStringWithLine(
          ">> [Board says] 한번에 4장 이상의 카드를 가져갑니다.");
      StealPIfPossible(opponent_player, answer);
      answer->push_back(card_from_player);
      answer->push_back(card_from_deck);
      for (Card* card : matched_cards) {
        answer->push_back(card);
      }

    } else if (matched_cards.size() == 1) {
      // 바닥에 1개의 카드가 겹침 ... 뻑
      View::PrintStringWithLine(">> [Board says] 뻑입니다.");
      floor_->push_back(card_from_player);
      floor_->push_back(card_from_deck);
      for (Card* card : matched_cards) {
        floor_->push_back(card);
      }

    } else if (matched_cards.size() == 0) {
      // 바닥에 0개의 카드가 겹침 ... 쪽
      View::PrintStringWithLine(">> [Board says] 쪽입니다.");
      StealPIfPossible(opponent_player, answer);
      answer->push_back(card_from_player);
      answer->push_back(card_from_deck);
    }
    
  } else {
    // 1. 플레이어가 낸 카드 처리
    std::vector<Card*> matched_cards_with_player_card =
        MatchedCardsWith(card_from_player);
    InsertCardsInto(opponent_player, matched_cards_with_player_card, answer);
    if (matched_cards_with_player_card.size() == 0) {
      floor_->push_back(card_from_player);
    } else {
      answer->push_back(card_from_player);
    }

    // 2. 덱에서 나온 카드 처리
    std::vector<Card*> matched_cards_with_deck_card =
        MatchedCardsWith(card_from_deck);
    InsertCardsInto(opponent_player, matched_cards_with_deck_card, answer);
    if (matched_cards_with_deck_card.size() == 0) {
      floor_->push_back(card_from_deck);
    } else {
      answer->push_back(card_from_deck);
    }
  }

  return answer;
}

// from에 있는 카드들을 경우에 따라 to에 옮겨 담는다.
void Board::InsertCardsInto(Player* opponent_player, const std::vector<Card*>& from, std::list<Card*>* to) {
  // 겹치는 카드가 없는 경우는 따로 처리한다.
  if (from.size() == 3 || from.size() == 1) {
    if (from.size() == 3) {
      View::PrintStringWithLine(">> [Board says] 한번에 4장 이상의 카드를 가져갑니다.");
      StealPIfPossible(opponent_player, to);
    }
    for (Card* card : from) {
      to->push_back(card);
    }

  } else if (from.size() == 2) {
    Card* card_0 = from[0];
    Card* card_1 = from[1];
    int player_choice = View::PrintChoicesAndGetCardOrder(card_0->ToString(), card_1->ToString());
    to->push_back(from[player_choice]);
    floor_->push_back(from[(player_choice+1) % 2]);
  }
}

// dack이 비어있는지 확인한다.
bool Board::DeckIsEmpty() { return deck_->empty(); }

// Getters
std::stack<Card*>* Board::deck() { return deck_; }

// Setters
void Board::set_floor(std::list<Card*>* new_floor) { floor_ = new_floor; }
void Board::set_deck(std::stack<Card*>* new_deck) { deck_ = new_deck; }

// card_from_player을 받은 Board가
// player에게 보내는 카드 리스트를 반환한다.
// std::list<Card*>* Board::CardsToPlayer(Card* card_from_player,
//                                     Player* opponent_player) {
//  std::list<Card*>* answer = new std::list<Card*>(); // delete
//  Controller::PlayOf
//
//  bool complete_for_player_card = false;
//  bool complete_for_deck_card = false;
//
//  Card* card_from_deck = DrawCardFromDeck();
//
//  if (!card_from_deck) delete card_from_deck;
//
//  View::PrintStringWithLine(">> [Board says] 플레이어가 낸 카드: " +
//                            card_from_player->ToString());
//  View::PrintStringWithLine(">> [Board says] 덱에서 나온 카드: " +
//                            card_from_deck->ToString());
//
//  std::vector<Card*> matched_cards_with_player =
//      MatchedCardWith(card_from_player);
//  if (static_cast<int>(matched_cards_with_player.size()) == 3) {
//    // 1-1. 플레이어 카드로 뭉쳐 있는 3장을 다 쓸어 가져간다.
//    View::PrintStringWithLine(
//        ">> [Board says] 한번에 4장 이상의 카드를 가져갑니다.");
//    complete_for_player_card = true;
//
//    answer->push_back(card_from_player);
//    for (Card* card : matched_cards_with_player) {
//      answer->push_back(card);
//    }
//    StealPIfPossible(opponent_player, answer);
//  }
//
//  std::vector<Card*> matched_cards_with_deck =
//  MatchedCardWith(card_from_deck); if
//  (static_cast<int>(matched_cards_with_deck.size()) == 3) {
//    // 1-2. 덱 카드로 뭉쳐 있는 3장을 다 쓸어 가져간다.
//    View::PrintStringWithLine(
//        ">> [Board says] 한번에 4장 이상의 카드를 가져갑니다.");
//    complete_for_deck_card = true;
//
//    answer->push_back(card_from_deck);
//    for (Card* card : matched_cards_with_player) {
//      answer->push_back(card);
//    }
//    StealPIfPossible(opponent_player, answer);
//  }
//  // CheckPoint 1. 플레이어 카드, 덱 카드에 대한 처리를 모두 마치면 끝낸다.
//  if (complete_for_player_card && complete_for_deck_card) return answer;
//
//  // 2-1. 플레이어 카드로 4장을 챙긴 후,
//  if (complete_for_player_card && !complete_for_deck_card) {
//    // 덱 카드에 대한 처리
//    if (static_cast<int>(matched_cards_with_deck.size()) == 2) {
//      answer->push_back(card_from_deck);
//      answer->push_back(matched_cards_with_deck[0]);
//      floor_->push_back(matched_cards_with_deck[1]);
//    }
//    if (static_cast<int>(matched_cards_with_deck.size()) == 1) {
//      answer->push_back(card_from_deck);
//      answer->push_back(matched_cards_with_deck[0]);
//    }
//    if (static_cast<int>(matched_cards_with_deck.size()) == 0) {
//      floor_->push_back(card_from_deck);
//    }
//    complete_for_deck_card = true;
//  }
//
//  // 2-2. 덱 카드로 4장을 챙긴 후,
//  if (complete_for_deck_card && !complete_for_player_card) {
//    // 플레이어 카드에 대한 처리
//    if (static_cast<int>(matched_cards_with_player.size()) == 2) {
//      answer->push_back(card_from_player);
//      answer->push_back(matched_cards_with_player[0]);
//      floor_->push_back(matched_cards_with_player[1]);
//    }
//    if (static_cast<int>(matched_cards_with_player.size()) == 1) {
//      answer->push_back(card_from_player);
//      answer->push_back(matched_cards_with_player[0]);
//    }
//    if (static_cast<int>(matched_cards_with_player.size()) == 0) {
//      floor_->push_back(card_from_player);
//    }
//    complete_for_player_card = true;
//  }
//  // CheckPoint 2. 플레이어 카드, 덱 카드에 대한 처리를 모두 마치면 끝낸다.
//  if (complete_for_player_card && complete_for_deck_card) return answer;
//
//  // 3-1. floor_에 플레이어의 카드와 일치하는 카드가 2장인 경우
//  if (static_cast<int>(matched_cards_with_player.size()) == 2) {
//    if (static_cast<int>(matched_cards_with_deck.size()) == 0 &&
//        card_from_player->month() == card_from_deck->month()) {
//      View::PrintStringWithLine(
//          ">> [Board says] 한번에 4장 이상의 카드를 가져갑니다.");
//      complete_for_player_card = true;
//      complete_for_deck_card = true;
//
//      answer->push_back(card_from_player);
//      answer->push_back(card_from_deck);
//      for (Card* card : matched_cards_with_player) {
//        answer->push_back(card);
//      }
//      StealPIfPossible(opponent_player, answer);
//
//    } else if (static_cast<int>(matched_cards_with_deck.size()) == 2) {
//      if (card_from_player->month() != card_from_deck->month()) {
//        complete_for_player_card = true;
//        complete_for_deck_card = true;
//
//        answer->push_back(card_from_player);
//        answer->push_back(card_from_deck);
//        answer->push_back(matched_cards_with_player[0]);
//        answer->push_back(matched_cards_with_deck[0]);
//
//        floor_->push_back(matched_cards_with_player[1]);
//        floor_->push_back(matched_cards_with_deck[1]);
//      }
//    } else if (static_cast<int>(matched_cards_with_deck.size()) == 1) {
//      complete_for_player_card = true;
//      complete_for_deck_card = true;
//
//      answer->push_back(card_from_player);
//      answer->push_back(card_from_deck);
//      answer->push_back(matched_cards_with_player[0]);
//      answer->push_back(matched_cards_with_deck[0]);
//
//      floor_->push_back(matched_cards_with_player[1]);
//
//    } else if (static_cast<int>(matched_cards_with_deck.size()) == 0) {
//      complete_for_player_card = true;
//      complete_for_deck_card = true;
//
//      answer->push_back(card_from_player);
//      answer->push_back(matched_cards_with_player[0]);
//
//      floor_->push_back(matched_cards_with_player[1]);
//      floor_->push_back(card_from_deck);
//    }
//  }
//  // CheckPoint 3-1. 플레이어 카드, 덱 카드에 대한 처리를 모두 마치면 끝낸다.
//  if (complete_for_player_card && complete_for_deck_card) return answer;
//
//  // 3-2. floor_에 덱 카드와 일치하는 카드가 2장인 경우
//  if (static_cast<int>(matched_cards_with_deck.size()) == 2) {
//    if (static_cast<int>(matched_cards_with_player.size()) == 1) {
//      complete_for_player_card = true;
//      complete_for_deck_card = true;
//
//      answer->push_back(card_from_player);
//      answer->push_back(matched_cards_with_player[0]);
//      answer->push_back(card_from_deck);
//      answer->push_back(matched_cards_with_deck[0]);
//
//      floor_->push_back(matched_cards_with_deck[1]);
//
//    } else if (static_cast<int>(matched_cards_with_player.size()) == 0) {
//      complete_for_player_card = true;
//      complete_for_deck_card = true;
//
//      answer->push_back(card_from_deck);
//      answer->push_back(matched_cards_with_deck[0]);
//
//      floor_->push_back(card_from_player);
//      floor_->push_back(matched_cards_with_deck[1]);
//    }
//  }
//  // CheckPoint 3-2. 플레이어 카드, 덱 카드에 대한 처리를 모두 마치면 끝낸다.
//  if (complete_for_player_card && complete_for_deck_card) return answer;
//
//  // 4. floor_에 플레이어 카드와 일치하는 카드가 1장인 경우
//  if (static_cast<int>(matched_cards_with_player.size()) == 1) {
//    if (static_cast<int>(matched_cards_with_deck.size()) == 0 &&
//        card_from_player->month() == card_from_deck->month()) {
//      // 뻑
//      View::PrintStringWithLine(">> [Board says] 뻑입니다.");
//      complete_for_player_card = true;
//      complete_for_deck_card = true;
//
//      floor_->push_back(card_from_player);
//      floor_->push_back(card_from_deck);
//      floor_->push_back(matched_cards_with_player[0]);
//
//    } else if (static_cast<int>(matched_cards_with_deck.size()) == 1) {
//      complete_for_player_card = true;
//      complete_for_deck_card = true;
//
//      answer->push_back(card_from_player);
//      answer->push_back(card_from_deck);
//      answer->push_back(matched_cards_with_player[0]);
//      answer->push_back(matched_cards_with_deck[0]);
//
//    } else if (static_cast<int>(matched_cards_with_deck.size()) == 0 &&
//               card_from_player->month() != card_from_deck->month()) {
//      complete_for_player_card = true;
//      complete_for_deck_card = true;
//
//      answer->push_back(card_from_player);
//      answer->push_back(matched_cards_with_player[0]);
//
//      floor_->push_back(card_from_deck);
//    }
//  }
//  // CheckPoint 4. 플레이어 카드, 덱 카드에 대한 처리를 모두 마치면 끝낸다.
//  if (complete_for_player_card && complete_for_deck_card) return answer;
//
//  // 5. floor_에 플레이어 카드와 일치하는 카드가 0장인 경우
//  if (static_cast<int>(matched_cards_with_player.size()) == 0) {
//    if (static_cast<int>(matched_cards_with_deck.size()) == 1 &&
//        card_from_player->month() != card_from_deck->month()) {
//      complete_for_player_card = true;
//      complete_for_deck_card = true;
//
//      answer->push_back(card_from_deck);
//      answer->push_back(matched_cards_with_deck[0]);
//
//      floor_->push_back(card_from_player);
//
//    } else if (static_cast<int>(matched_cards_with_deck.size()) == 0 &&
//               card_from_player->month() != card_from_deck->month()) {
//      complete_for_player_card = true;
//      complete_for_deck_card = true;
//
//      floor_->push_back(card_from_player);
//      floor_->push_back(card_from_deck);
//    } else if (static_cast<int>(matched_cards_with_deck.size()) == 0 &&
//               card_from_player->month() == card_from_deck->month()) {
//      // 쪽
//      View::PrintStringWithLine(">> [Board says] 쪽 입니다.");
//      complete_for_player_card = true;
//      complete_for_deck_card = true;
//
//      answer->push_back(card_from_player);
//      answer->push_back(card_from_deck);
//      StealPIfPossible(opponent_player, answer);
//    }
//  }
//  // CheckPoint 5. 플레이어 카드, 덱 카드에 대한 처리를 모두 마치면 끝낸다.
//  if (complete_for_player_card && complete_for_deck_card) return answer;
//
//  return answer;
//}
