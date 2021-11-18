#include "pocket.h"

#include <string>

// Constructor
Pocket::Pocket() {
  pocket_for_gwang_ = new std::list<Card*>();
  pocket_for_animals_ = new std::list<Card*>();
  pocket_for_dan_ = new std::list<Card*>();
  pocket_for_p_ = new std::list<Card*>();
}

// Destuctor
Pocket::~Pocket() {
  delete pocket_for_gwang_;
  delete pocket_for_animals_;
  delete pocket_for_dan_;
  delete pocket_for_p_;
}

// 각 주머니의 카드를 삭제(delete)한다.
void Pocket::DeleteAllCardsInEachPocket() {
  if (pocket_for_gwang_) {
    for (Card* card_gwang : *pocket_for_gwang_) {
      delete card_gwang;
    }
  }
  if (pocket_for_animals_) {
    for (Card* card_animal : *pocket_for_animals_) {
      delete card_animal;
    }
  }
  if (pocket_for_dan_) {
    for (Card* card_dan : *pocket_for_dan_) {
      delete card_dan;
    }
  }
  if (pocket_for_p_) {
    for (Card* card_p : *pocket_for_p_) {
      delete card_p;
    }
  }
}

// 주머니에 카드를 넣는다.
void Pocket::AddCard(Card* a_card) {
  std::string card_property = a_card->property();
  if (card_property.compare("gwang") == 0) {
    AddGwang(a_card);
  } else if (card_property.compare("animal") == 0) {
    AddAnimal(a_card);
  } else if (card_property.compare("dan") == 0) {
    AddDan(a_card);
  } else if (card_property.compare("p") == 0) {
    AddP(a_card);
  }
}

// 지금까지 플레이어가 얻어낸 카드들을 문자열로 얻는다.
const std::string Pocket::ToStringForPockets() {
  // player's current card list
  return "--------------------------------\n"
         "-- player\'s current card list --\n"
         "--------------------------------\n" +
         ToStringForGwangPocket() + ToStringForAnimalsPocket() +
         ToStringForDanPocket() + ToStringForPPocket();
}

// 피를 담는 주머니에서 피를 하나 꺼낸다.
Card* Pocket::GetPIfPossible() {
  if (PocketPIsEmpty()) {
    return nullptr;
  } else {
    Card* answer = pocket_for_p_->front();
    pocket_for_p_->pop_front();
    return answer;
  }
}

// Getters
const std::list<Card*>* Pocket::pocket_for_gwang() { return pocket_for_gwang_; }
const std::list<Card*>* Pocket::pocket_for_animals() {
  return pocket_for_animals_;
}
const std::list<Card*>* Pocket::pocket_for_dan() { return pocket_for_dan_; }
const std::list<Card*>* Pocket::pocket_for_p() { return pocket_for_p_; }

// ---------------------------------------------------
// ----------------- Private methods -----------------
// ---------------------------------------------------

// 광 주머니의 정보를 문자열로 반환한다.
const std::string Pocket::ToStringForGwangPocket() {
  std::string answer = "  Gwang : ";
  std::list<Card*>::iterator iterator_for_gwang;
  int count = 0;

  for (iterator_for_gwang = pocket_for_gwang_->begin();
       iterator_for_gwang != pocket_for_gwang_->end(); iterator_for_gwang++) {
    answer += "| " + (*iterator_for_gwang)->ToString() + " ";
    count++;
    if (count % 5 == 0) {
      answer += "|\n";
    }
  }
  if (count % 5 != 0) {
    answer += "|\n";
  }

  return answer;
}

// 동물 주머니의 정보를 문자열로 반환한다.
const std::string Pocket::ToStringForAnimalsPocket() {
  std::string answer = "\n Animal : ";
  std::list<Card*>::iterator iterator_for_Animals;
  int count = 0;

  for (iterator_for_Animals = pocket_for_animals_->begin();
       iterator_for_Animals != pocket_for_animals_->end();
       iterator_for_Animals++) {
    answer += "| " + (*iterator_for_Animals)->ToString() + " ";
    count++;
    if (count % 5 == 0) {
      answer += "|\n\t  ";
    }
  }
  if (count % 5 != 0) {
    answer += "|\n";
  }
  // answer += "==============================";

  return answer;
}

// 단 주머니의 정보를 문자열로 반환한다.
const std::string Pocket::ToStringForDanPocket() {
  std::string answer = "\n    Dan : ";
  std::list<Card*>::iterator iterator_for_Dan;
  int count = 0;

  for (iterator_for_Dan = pocket_for_dan_->begin();
       iterator_for_Dan != pocket_for_dan_->end(); iterator_for_Dan++) {
    answer += "| " + (*iterator_for_Dan)->ToString() + " ";
    count++;
    if (count % 5 == 0) {
      answer += "|\n\t  ";
    }
  }
  if (count % 5 != 0) {
    answer += "|\n";
  }

  // answer += "==============================";

  return answer;
}
// 피 주머니의 정보를 문자열로 반환한다.
const std::string Pocket::ToStringForPPocket() {
  std::string answer = "\n      P : ";
  std::list<Card*>::iterator iterator_for_p;
  int count = 0;

  for (iterator_for_p = pocket_for_p_->begin();
       iterator_for_p != pocket_for_p_->end(); iterator_for_p++) {
    answer += "| " + (*iterator_for_p)->ToString() + " ";
    count++;
    if (count % 5 == 0) {
      answer += "|\n\t  ";
    }
  }

  if (count != 0 && count % 5 != 0) {
    answer += "|";
  }
  answer += "\n--------------------------------";

  return answer;
}

// 카드 식별 후, 맞는 주머니에 카드를 넣는다.
void Pocket::AddGwang(Card* a_card) { pocket_for_gwang_->push_back(a_card); }
void Pocket::AddAnimal(Card* a_card) { pocket_for_animals_->push_back(a_card); }
void Pocket::AddDan(Card* a_card) { pocket_for_dan_->push_back(a_card); }
void Pocket::AddP(Card* a_card) { pocket_for_p_->push_back(a_card); }

// 피를 담는 주머니가 비어있는지 확인한다.
bool Pocket::PocketPIsEmpty() { return pocket_for_p_->empty(); }
