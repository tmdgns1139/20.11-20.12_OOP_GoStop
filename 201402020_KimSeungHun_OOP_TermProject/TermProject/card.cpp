#include "card.h"
#include "view.h"

#include <iostream>
#include <string>

// Constructors
Card::Card() : month_(0), property_("\0"), special_("\0") {}

Card::Card(int month, std::string property, std::string special)
    : month_(month), property_(property), special_(special) {
  if (month < 0 || month >= 13)
    View::PrintStringWithLine(
        "\n[ERROR] card_month should be 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12\n");
}

// Destructor
Card::~Card() {}
// 소멸자 호출 확인을 위한 디버깅 메시지
//View::PrintStringWithLine("[Card says] ~Card(" + std::to_string(month_) +
//                          ") is called!");

int Card::month() { return month_; }
const std::string& Card::property() { return property_; }
const std::string& Card::special() { return special_; }
const std::string& Card::special() const { return special_; }

bool Card::IsCardValid() { return ((month_ >= 1) && (month_ <= 12)); }

std::string Card::ToString() {
  return std::to_string(month_) + "(" + property_ + "_" + special_ + ")";
}
