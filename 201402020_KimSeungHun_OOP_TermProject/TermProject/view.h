#ifndef _VIEW_H_
#define _VIEW_H_

#include <string>

class View {
 public:
  static void PrintScore(int a_score);
  static int PrintChoicesAndGetCardOrder(std::string card_1_info,
                                         std::string card_2_info);

  static void PrintStringWithLine(std::string a_string);
  static void PrintStringWithoutLine(std::string a_string);

  static void PrintPlayerAvailableCards(std::string available_cards_info);
  static void PrintPlayerAllGettingCards(std::string all_getting_cards_info);

  static void PrintBoardDeck(std::string deck_info);
  static void PrintBoardfloor(std::string floor_info);

  static std::string InputString();
  static char InputChar();
  static int InputInt();
  
  static char InputUserAnswerAboutGameStart();
  static char InputUserAnswerAboutGo();
  static int InputOrderFromPlayerAvailableCards(int bound);

 private:
  View();
};

#endif  // _VIEW_H_
