#include "view.h"

#include <iostream>
#include <string>

void View::PrintScore(int a_score) {
  View::PrintStringWithLine(
      "해당 플레이어의 현재 점수 : " + std::to_string(a_score) + "점.");
}

int View::PrintChoicesAndGetCardOrder(std::string card_0_info, std::string card_1_info) {
  View::PrintStringWithLine(">> 겹치는 카드 발생!");
  View::PrintStringWithLine(">> 0 -> [ " + card_0_info + " ]");
  View::PrintStringWithLine(">> 1 -> [ " + card_1_info + " ]");
  View::PrintStringWithoutLine(">> 둘 중 한 카드를 골라 가져가세요. : ");
  int answer = View::InputInt();
  while (true) {
    if (answer == 0 || answer == 1) {
      return answer;

    } else {
      View::PrintStringWithoutLine(">> [다시 입력] 둘 중 한 카드(0 또는 1)를 골라 가져가세요. : ");
    }
  }
}

void View::PrintPlayerAvailableCards(std::string available_cards_info) {
  View::PrintStringWithLine(available_cards_info);
}
void View::PrintPlayerAllGettingCards(std::string all_getting_cards_info) {
  View::PrintStringWithLine(all_getting_cards_info);
}

void View::PrintBoardDeck(std::string deck_info) {
  View::PrintStringWithLine(deck_info);
}
void View::PrintBoardfloor(std::string floor_info) {
  View::PrintStringWithLine(floor_info);
}

void View::PrintStringWithLine(std::string a_string) {
  std::cout << a_string << std::endl;
}
void View::PrintStringWithoutLine(std::string a_string) {
  std::cout << a_string;
}

std::string View::InputString() {
  std::string answer = "";
  std::cin >> answer;
  return answer;
}
char View::InputChar() {
  std::string answer = View::InputString();
  char answer_char = answer.at(0);
  return answer_char;
}

char View::InputUserAnswerAboutGameStart() {
  PrintStringWithoutLine(
      "\n!! 게임을 시작하려면 \'Y\'또는 \'y\'를 입력하세요.(종료하려면 "
      "아무키나 누르세요.) : ");
  return View::InputChar();
}

char View::InputUserAnswerAboutGo() {
  PrintStringWithoutLine(
      "\n!! [ANNOUNCE] Go를 하려면 \'Y\'또는 \'y\'를 입력하세요.(Stop하려면 "
      "아무키나 누르세요.) : ");
  return View::InputChar();
}

int View::InputInt() {
  int answer = 0;
  while (true) {
    std::cin >> answer;
    if (std::cin.fail()) {  // 1. std::cin 입력이 정상적으로 처리못한 경우,
      std::cin.clear();           // 2. std::cin 상태를 초기화하고
      fseek(stdin, 0, SEEK_END);  // 3. 입력 버퍼를 초기화한다.
      View::PrintStringWithoutLine(">> [다시 입력] 숫자를 입력해야합니다.");

      continue;

    } else {
      return answer;
    }
  }
}

int View::InputOrderFromPlayerAvailableCards(int bound) {
  View::PrintStringWithoutLine(">> 낼 카드의 인덱스를 입력하세요. : ");
  while (true) {
    int answer = View::InputInt();

    if (answer < 0 || answer >= bound)
      View::PrintStringWithoutLine(
          ">> [다시 입력] 낼 수 있는 카드들의 인덱스를 벗어났습니다. : ");
    else
      return answer;
  }
}

View::View() {}
