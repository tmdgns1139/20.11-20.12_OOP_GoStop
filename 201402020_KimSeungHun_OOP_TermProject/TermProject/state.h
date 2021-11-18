#ifndef _STATE_H_
#define _STATE_H_

class State {
 public:
  // Constructor
  State();
  // Destructor
  ~State();

  // 고를 외칠때마다 고 횟수를 1 올리고 
  // 승리 최소 점수도 고를 외친 시점의 점수에서 1을 올린다.
  void ChangeStateForGo(int score_when_declare_go);

  // Getters
  const int number_of_go();
  const int std_for_win();

  // Setters
  void set_number_of_go(int new_number_of_go);
  void set_std_for_win(int new_std_score_for_win);

 private:
  int number_of_go_; // 고를 외친 횟수
  int std_score_for_win_; // 승리하기 위해 얻을 최소 점수
};

#endif  // _STATE_H_
