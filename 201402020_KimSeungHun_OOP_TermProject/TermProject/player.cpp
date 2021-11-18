#include "player.h"

// Constructor
Player::Player() {
  available_cards_ = new std::vector<Card*>();
  pockets_for_cards_ = new Pocket();
  current_state_ = new State();
}

// Destructor
Player::~Player() {
  delete available_cards_;
  delete pockets_for_cards_;
  delete current_state_;
}

// 주머니에 카드를 넣는다.
void Player::AddCard(Card* a_card) { pockets_for_cards_->AddCard(a_card); }

// 주머니에서 피카드를 하나 꺼낸다.
Card* Player::GetPIfPossible() { return pockets_for_cards_->GetPIfPossible(); }

// 고를 선언하고 현재 상태를 갱신한다.
void Player::DeclareGo(int score_when_declare_go) {
  current_state_->ChangeStateForGo(score_when_declare_go);
}

// 현재 플레이어가 낼 수 있는 카드를 문자열로 얻는다.
const std::string Player::ToStringForAvailableCards() {
  std::string answer = "";
  int index = 0;
  for (Card* card : *available_cards_) {
    answer += "| " + card->ToString() + "_\'" + std::to_string(index) + "\' ";
    index++;
    if (index % 5 == 0) {
      answer += "|\n";
    }
  }
  if (index % 5 != 0) answer += " |";
  // answer += "\n----------------------------------";
  return answer;
}

// 주머니에 있는 카드들을 정보를 문자열로 반환한다.
const std::string Player::ToStringForPockets() {
  return pockets_for_cards_->ToStringForPockets();
}

// 플레이어가 낼 카드를 선택하여 낸다.
// 심판은 플레이어가 한 턴에 한번만 카드를 내도록 통제한다.
Card* Player::DropCard(int selected_card_order) {
  Card* answer = (*available_cards_)[selected_card_order];
  available_cards_->erase(available_cards_->begin() + selected_card_order);
  return answer;
}

// 플레이어의 주머니와 상태를 삭제하고 다시 만든다. (판마다 호출된다.)
void Player::CleanPocketsAndResetScore() {
  if (pockets_for_cards_) {
    pockets_for_cards_->DeleteAllCardsInEachPocket();
  }

  delete pockets_for_cards_;
  delete current_state_;

  pockets_for_cards_ = new Pocket(); // delete in Player::CleanPocketsAndResetScore
  current_state_ = new State(); // delete in Player::CleanPocketsAndResetScore

}

// Getters
std::vector<Card*>* Player::available_cards() { return available_cards_; }
Pocket* Player::pockets_for_cards() { return pockets_for_cards_; }
State* Player::current_state() { return current_state_; }

// Setters
void Player::set_avilable_cards(std::vector<Card*>* new_available_cards) {
  available_cards_ = new_available_cards;
}
