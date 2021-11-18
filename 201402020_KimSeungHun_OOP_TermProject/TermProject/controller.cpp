#include "controller.h"

#include <iostream>
#include <list>

//#include "player.h"
#include "view.h"

// Constructor
Controller::Controller() : player_A_(), player_B_(), board_(), judge_(){}

// Destructor
Controller::~Controller() {}

// a_player의 차례이다.
void Controller::PlayOf(Player* a_player, Player* opponent_player) {
  // 매 차례마다 deck과 floor의 정보를 출력한다.
  View::PrintBoardDeck(board_.ToStringForDeck());
  View::PrintBoardfloor(board_.ToStringForFloor());

  // 카드를 내기 전, 얻어둔 카드를 출력한다.
  View::PrintPlayerAllGettingCards(a_player->ToStringForPockets());

  // 플레이어가 낼 수 있는 카드 정보를 출력한다.
  View::PrintStringWithLine(
      "\nAvailable Cards(month_property_specieal_ORDER) : ");
  View::PrintPlayerAvailableCards(a_player->ToStringForAvailableCards());

  // 사용자로부터 숫자를 입력받아 카드를 낸다.
  int player_choice_card_order = View::InputOrderFromPlayerAvailableCards(
      static_cast<int>(a_player->available_cards()->size()));
  Card* selected_card = a_player->DropCard(player_choice_card_order);

  // 1. 낸 카드를 가지고 얻을 수 있는 카드들을 플레이어의 주머니에 넣는다.
  // 2. 따낸 카드 목록을 출력한다.
  std::list<Card*>* to_player =
      board_.CardsToPlayer(selected_card, opponent_player);
  View::PrintStringWithoutLine("!! 플레이어가 가져간 카드 목록 : ");
  if (to_player->empty()) {
    View::PrintStringWithLine("없음.");
  } else {
    for (Card* card : *to_player) {
      View::PrintStringWithoutLine(card->ToString() + ", ");
      a_player->AddCard(card);
    }
    View::PrintStringWithLine("끝.");
  }
  delete to_player; // new in Board::CardsToPlayer

  // 카드를 낸 후, 얻어둔 카드를 출력한다.
  View::PrintPlayerAllGettingCards(a_player->ToStringForPockets());

  // 점수를 계산하고 출력한다.
  int score_of_player = judge_.CalcScore(a_player, opponent_player);
  score_of_player += a_player->current_state()->number_of_go();
  View::PrintScore(score_of_player);

  // 점수가 7점 이상이면 해당 플레이어의 승리로 간주하고 심판의 게임 종료 선언이
  // 이루어진다.
  judge_.set_is_game_over(judge_.IsGameOver(score_of_player));

  if (judge_.is_game_over()) {
    char answer_about_go = View::InputUserAnswerAboutGo();
    if (answer_about_go == 'Y' || answer_about_go == 'y') {
      judge_.set_is_game_over(false);
      a_player->DeclareGo(score_of_player);
    }
  }

  // 차례 종료 후, pause를 걸어 결과를 확인하도록 한다.
  View::PrintStringWithoutLine("[ 결과를 확인하고 넘어가세요. ]");
  system("PAUSE");
}

// 1. 카드 만들기 (48장)
// 2. 카드를 플레이어에게 10장씩 나누어주기.
// 3. 카드 8장을 판의 바닥에 깔아두기.
void Controller::InitSetting() {
  // deck을 비운다.
  judge_.ClearDeck(board_.deck());
  //judge_.Cl

  // 플레이어가 기존에 들고 있던 패를 모두 비우고 점수를 0점으로 초기화한다.
  player_A_.CleanPocketsAndResetScore();
  player_B_.CleanPocketsAndResetScore();

  // Board deck setting
  board_.set_deck(judge_.InitDeck());

  // Player setting
  player_A_.set_avilable_cards(judge_.CardsForPlayerFromInitDeck(board_.deck()));
  player_B_.set_avilable_cards(judge_.CardsForPlayerFromInitDeck(board_.deck()));

  // Board floor setting
  board_.set_floor(judge_.CardsForBoardFloor(board_.deck()));

  judge_.set_is_game_over(false);
}

// 게임을 한판 진행한다.
void Controller::PlayGame() {
  InitSetting();
  while (!judge_.is_game_over()) {
    // A 턴
    View::PrintStringWithLine("\n>>> Player A의 차례입니다.");
    PlayOf(&player_A_, &player_B_);
    if (judge_.is_game_over()) {
      View::PrintStringWithLine(
          "\n[ANNOUNCE] Player A의 승리로 게임을 종료합니다!");
      break;
    }

    // B 턴
    View::PrintStringWithLine("\n>>> Player B의 차례입니다.");
    PlayOf(&player_B_, &player_A_);
    if (judge_.is_game_over()) {
      View::PrintStringWithLine(
          "\n[ANNOUNCE] Player B의 승리로 게임을 종료합니다!");
      break;
    }

    if (board_.DeckIsEmpty()) {
      judge_.set_is_game_over(true);
      View::PrintStringWithLine("\n[ANNOUNCE] 무승부로 게임을 종료합니다!");
    }
  }
}

void Controller::Run() {
  char answer = View::InputUserAnswerAboutGameStart();
  while (answer == 'Y' || answer == 'y') {
    PlayGame();
    answer = View::InputUserAnswerAboutGameStart();
  }
}
