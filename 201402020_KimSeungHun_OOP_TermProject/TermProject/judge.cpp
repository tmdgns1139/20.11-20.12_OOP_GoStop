#include "judge.h"

#include <time.h>

#include <random>
#include <string>
#include <vector>

#include "view.h"

// 아래 매크로와 주석은 cpplint에서 경고를 받았지만
// 가독성을 위해 이대로 두었습니다.

#define NUMBER_OF_CARDS 48                  // 게임에 쓰이는 카드는 총 48장이다.
#define NUMBER_OF_CARDS_FOR_PLAYER 10       // 각 플레이어는 10장의 카드를 받는다.
#define NUMBER_OF_CARDS_FOR_BOARD_FLOOR 8   // 바닥에는 8장의 카드를 깔아놓는다.

#define STD_OF_SCORE_WITH_P 15      // 피가 15장 이상일 경우, 점수를 낸다.
#define STD_OF_SCORE_WITH_DAN 5     // 단이 5장 이상일 경우, 점수를 낸다.
#define STD_OF_SCORE_WITH_ANIMAL 5  // 동물이 5장 이상일 경우, 점수를 낸다.
#define STD_OF_SCORE_WITH_GWANG 3   // 광이 3장 이상일 경우, 점수를 낸다.

#define STD_OF_DOUBLE_SCORE_WITH_ANIMALS 8  // 동물이 8장 이상일 경우, 멍따로 총 점수의 2배를 해준다.
#define STD_OF_P_BAK 7                      // 피로 점수를 낸 경우, 상대방의 피가 7장 미만이면 피박이 발생한다.
#define STD_OF_SCORE_FOR_WIN 7              // 7점 이상의 점수를 내면 이긴다.
#define STD_OF_GODORI_WITH_BIRD 3           // 고도리는 새를 3장 모은 경우로 5점을 획득한다.

#define SCORE_GODIRI 5                      // 고도리는 5점이다.
#define SCORE_RED_DAN 3                     // 홍단은 3점이다.
#define SCORE_BLUE_DAN 3                    // 청단은 3점이다.
#define SCORE_GRASS_DAN 3                   // 초단은 3점이다.
#define SCORE_THREE_GWANG_WITH_RAIN 2       // 비광 포함 3개의 광은 2점이다.
#define SCORE_THREE_GWANG_WITHOUT_RAIN 3    // 비광 미포함 3개의 광은 2점이다.

#define INCREMENT_FOR_DOUBLE_P 2  // 쌍피의 피 증가는 2이다.

// Constructor
Judge::Judge() : is_game_over_(false) {}

// Destructor
Judge::~Judge() {}

// player에게 10장의 카드를 빼놓는다.
std::vector<Card*>* Judge::CardsForPlayerFromInitDeck(std::stack<Card*>* a_deck) {
  std::vector<Card*>* answer = new std::vector<Card*>();
  View::PrintStringWithLine(
      ">>> [Judge says] 플레이어에게 10장의 카드를 분배합니다.");
  for (int i = 0; i < NUMBER_OF_CARDS_FOR_PLAYER; i++) {
    answer->push_back(a_deck->top());
    a_deck->pop();
  }

  return answer;
}

// board의 바닥에 깔 8장의 카드를 빼놓는다.
std::list<Card*>* Judge::CardsForBoardFloor(std::stack<Card*>* a_deck) {
  std::list<Card*>* answer = new std::list<Card*>();
  View::PrintStringWithLine(">>> [Judge says] 바닥에 8장의 카드를 내려둡니다.");
  for (int i = 0; i < NUMBER_OF_CARDS_FOR_BOARD_FLOOR; i++) {
    answer->push_back(a_deck->top());
    a_deck->pop();
  }

  return answer;
}

// 초기 덱(deck)을 생성한다.
std::stack<Card*>* Judge::InitDeck() { return Suffle(MakeAllCards()); }

// 덱에 있는 카드와 덱을 삭제한다.
void Judge::ClearDeck(std::stack<Card*>* a_deck) { 
  if (a_deck) {
    while (!a_deck->empty()) {
      delete a_deck->top(); // new in Judge::MakeAllCards()
      a_deck->pop();
    }
    delete a_deck;  // new in Judge::Shuffle()
  }
}

// 바닥(floor_)에 있는 카드와 바닥을 삭제한다.
void Judge::ClearFloor(std::list<Card*>* a_floor) {
  if (a_floor) {
    for (Card* card_in_floor : *a_floor) {
      delete card_in_floor;
    }
    delete a_floor;
  }
}

// 광에 대한 점수를 계산한다.
int Judge::CalcScoreForGwang(Player* a_player) {
  int answer = 0;

  int count_gwang = 0;
  bool has_rain_gwang = false;

  const std::list<Card*>* gwangs = a_player->pockets_for_cards()->pocket_for_gwang();

  for (const Card* card : *gwangs) {
    count_gwang++;
    if (card->special().compare("rain") == 0) {
      has_rain_gwang = true;
    }
  }
  if (count_gwang == 3) {
    if (has_rain_gwang)
      answer += SCORE_THREE_GWANG_WITH_RAIN;
    else
      answer += SCORE_THREE_GWANG_WITHOUT_RAIN;

  } else if (count_gwang >= 4) {
    answer += count_gwang;
  }

  return answer;
}
// 동물에 대한 점수를 계산한다.
int Judge::CalcScoreForAnimal(Player* a_player, bool& is_mung) {
  int answer = 0;

  int count_animal = 0;
  int count_bird = 0;

  const std::list<Card*>* animals = a_player->pockets_for_cards()->pocket_for_animals();

  for (const Card* card : *animals) {
    count_animal++;
    if (card->special().compare("bird") == 0) count_bird++;
  }
  if (count_bird == STD_OF_GODORI_WITH_BIRD) answer += SCORE_GODIRI;
  if (count_animal >= STD_OF_SCORE_WITH_ANIMAL)
    answer += (count_animal - STD_OF_SCORE_WITH_ANIMAL + 1);

  // 멍따 : 총점수에 곱하기 2 ... !
  if (count_animal >= STD_OF_DOUBLE_SCORE_WITH_ANIMALS) is_mung = true;

  return answer;
}
// 단에 대한 점수를 계산한다.
int Judge::CalcScoreForDan(Player* a_player) {
  int answer = 0;

  const std::list<Card*>* dan = a_player->pockets_for_cards()->pocket_for_dan();

  int count_dan = 0;
  int count_red_dan = 0;
  int count_blue_dan = 0;
  int count_grass_dan = 0;

  for (const Card* card : *dan) {
    count_dan++;
    if (card->special().compare("red") == 0) {
      count_red_dan++;
    } else if (card->special().compare("blue") == 0) {
      count_blue_dan++;
    } else if (card->special().compare("grass") == 0) {
      count_grass_dan++;
    }
  }
  if (count_dan >= STD_OF_SCORE_WITH_DAN)
    answer += (count_dan - STD_OF_SCORE_WITH_DAN + 1);
  
  // 홍단 : 3점
  if (count_red_dan == 3)
    answer += SCORE_RED_DAN;
  // 청단 : 3점
  if (count_blue_dan == 3)
    answer += SCORE_BLUE_DAN;
  // 초단 : 3점
  if (count_grass_dan == 3)
    answer += SCORE_GRASS_DAN;
  
  return answer;
}

// 피에 대한 점수를 계산한다.
int Judge::CalcScoreForP(Player* a_player) {
  int answer = 0;
  int count_p = 0;

  const std::list<Card*>* p = a_player->pockets_for_cards()->pocket_for_p();

  for (const Card* card : *p) {
    if (card->special().compare("double") == 0)
      count_p += INCREMENT_FOR_DOUBLE_P;
    else
      count_p++;
  }
  if (count_p >= STD_OF_SCORE_WITH_P)
    answer += (count_p - STD_OF_SCORE_WITH_P + 1);

  return answer;
}

// 플레이어의 점수를 계산한다.
int Judge::CalcScore(Player* a_player, Player* opponent_player) {
  int answer = 0;
  
  // 1. 광에 대한 점수 계산을 하고 가산한다.
  int score_for_gwang = CalcScoreForGwang(a_player);    // 광박 판단용 변수
  answer += score_for_gwang;

  // 2. 단에 대한 점수 계산을 하고 가산한다.
  answer += CalcScoreForDan(a_player);

  // 3. 피에 대한 점수 계산을 하고 가산한다.
  int score_for_p = CalcScoreForP(a_player);    // 피박 판단용 변수
  answer += score_for_p;

  // 4-1. 동물에 대한 점수 계산을 하고 가산한다.
  bool is_mung = false; // 멍따 판단용 변수
  answer += CalcScoreForAnimal(a_player, is_mung);

  // 4-2. 멍따(동물 8장 이상 수집) : 현재 점수에 곱하기 2 ... !
  if (is_mung) {
    answer *= 2;
    View::PrintStringWithLine("[Judge says] 멍따로 두배의 점수를 얻습니다.");
  }

  // 5. 점수를 7점 이상 내면 광박, 피박을 따져본다.
  if (answer >= STD_OF_SCORE_FOR_WIN) {
    // 광박 : 총점수에 곱하기 2 ... !
    if (score_for_gwang >= 1 && opponent_player->pockets_for_cards()->pocket_for_gwang()->empty()) {
      answer *= 2;
      View::PrintStringWithLine(
          "[Judge says] 상대의 광박으로 두배의 점수를 얻습니다.");
    }

    // 피박 : 총점수에 곱하기 2 ... !
    if (score_for_p >= 1 &&
        opponent_player->pockets_for_cards()->pocket_for_p()->size() < STD_OF_P_BAK) {
      answer *= 2;
      View::PrintStringWithLine(
          "[Judge says] 상대의 피박으로 두배의 점수를 얻습니다.");
    }

  }

  return answer;
}

// 점수가 7점 이상이면 게임을 종료한다.
bool Judge::IsGameOver(int a_score) {
  if (a_score >= STD_OF_SCORE_FOR_WIN)
    return true;
  else
    return false;
}

// Getters & Setters
bool Judge::is_game_over() { return is_game_over_; }
void Judge::set_is_game_over(bool new_is_game_over) {
  is_game_over_ = new_is_game_over;
}

// -------------------------------------
// ---------- Private methods ----------
// -------------------------------------

// 게임에 쓸 카드 48장을 만든다.
std::vector<Card*>* Judge::MakeAllCards() {
  std::string all_cards_properties[] = {
      "gwang",  "dan", "p", "p", "animal", "dan",    "p",   "p",
      "gwang",  "dan", "p", "p", "animal", "dan",    "p",   "p",
      "animal", "dan", "p", "p", "animal", "dan",    "p",   "p",
      "animal", "dan", "p", "p", "gwang",  "animal", "p",   "p",
      "animal", "dan", "p", "p", "animal", "dan",    "p",   "p",
      "gwang",  "p",   "p", "p", "gwang",  "animal", "dan", "p"};
  std::string all_cards_special[] = {
      "normal", "red",    "normal", "normal", "bird",   "red",    "normal",
      "normal", "normal", "red",    "normal", "normal", "bird",   "grass",
      "normal", "normal", "normal", "grass",  "normal", "normal", "normal",
      "blue",   "normal", "normal", "normal", "grass",  "normal", "normal",
      "normal", "bird",   "normal", "normal", "normal", "blue",   "normal",
      "normal", "normal", "blue",   "normal", "normal", "normal", "double",
      "normal", "normal", "rain",   "normal", "normal", "double"};

  View::PrintStringWithLine(
      "\n>>> [Judge says] 게임에 사용할 카드 48장을 새로 생성합니다.");
  std::vector<Card*>* a_card_list = new std::vector<Card*>(); // delete in Judge::Suffle
  for (int index = 0; index < NUMBER_OF_CARDS; index++) {
    int month = (index / 4) + 1;
    a_card_list->push_back(
        new Card(month, all_cards_properties[index], all_cards_special[index])); 
    // delete in ... 덱, 바닥, 플레이어의 주머니를 비우면서 없어진다.
  }

  return a_card_list;
}

// 만들어진 48장의 카드를 섞는다.
std::stack<Card*>* Judge::Suffle(std::vector<Card*>* all_cards) {
  std::stack<Card*>* deck = new std::stack<Card*>();  // delete in Judge::ClearDeck()
  View::PrintStringWithLine(">>> [Judge says] 카드를 섞습니다.");

  srand((unsigned int)time(0));
  for (int i = 0; i < NUMBER_OF_CARDS; i++) {
    int random_index = rand() % all_cards->size();
    deck->push((*all_cards)[random_index]);
    all_cards->erase(all_cards->begin() + random_index);
  }

  delete all_cards;  // new in Judge::MakeAllCards()

  return deck;
}
