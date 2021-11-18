#ifndef _POCKET_H_
#define _POCKET_H_

#include <list>

#include "card.h"

class Pocket {
 public:
  // Constructor
  Pocket();

  // Destuctor
  ~Pocket();

  // 각 주머니의 카드를 삭제(delete)한다.
  void DeleteAllCardsInEachPocket();

  // 주머니에 카드를 넣는다.
  void AddCard(Card* a_card);
  
  // 주머니에 있는 카드들을 문자열로 얻는다.
  const std::string ToStringForPockets();

  // 피를 담는 주머니에서 피를 하나 꺼낸다.
  Card* GetPIfPossible();

  // Getters
  const std::list<Card*>* pocket_for_gwang();
  const std::list<Card*>* pocket_for_animals();
  const std::list<Card*>* pocket_for_dan();
  const std::list<Card*>* pocket_for_p();

 private:
  std::list<Card*>* pocket_for_gwang_;
  std::list<Card*>* pocket_for_animals_;
  std::list<Card*>* pocket_for_dan_;
  std::list<Card*>* pocket_for_p_;

  // 광, 동물, 단, 피 주머니에 있는 문자열을 얻는다.
  const std::string ToStringForGwangPocket();
  const std::string ToStringForAnimalsPocket();
  const std::string ToStringForDanPocket();
  const std::string ToStringForPPocket();

  // 카드 식별 후, 맞는 주머니에 카드를 넣는다.
  void AddGwang(Card* a_card);
  void AddAnimal(Card* a_card);
  void AddDan(Card* a_card);
  void AddP(Card* a_card);

  // 피를 담는 주머니가 비어있는지 확인한다.
  bool PocketPIsEmpty();
};

#endif  // _POCKET_H_
