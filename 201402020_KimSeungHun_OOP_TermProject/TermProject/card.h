#ifndef _CARD_H_
#define _CARD_H_

#include <string>

class Card {
 public:
  // Constructors
  Card();
  Card(int month, std::string property, std::string special);

  // Destructor
  ~Card();

  // 카드(this)의 유효성을 판단한다.
  bool IsCardValid();

  // 카드의 정보를 문자열로 반환한다.
  std::string ToString();

  // Getters
  int month();
  const std::string& property();
  const std::string& special();
  const std::string& special() const;

 private:
  const int month_;
  const std::string property_;  // gwang, dan, animal, p
  const std::string special_;  // normal except bird(고도리), blue(청단), red(홍단),
                         // rain(비광), double(쌍피)
};

#endif  // _CARD_H_
