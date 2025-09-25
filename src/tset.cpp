// Множество - реализация через битовые поля

#include <vector>
#include "tset.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : bitField(mp)
{
}

// конструктор копирования
TSet::TSet(const TSet& s) : bitField(s.bitField)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : bitField(bf)
{
}

TSet::operator TBitField()
{
  return bitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
  return bitField.GetLength();
}

bool TSet::IsMember(const int elem) const // элемент множества?
{
  return bitField.GetBit(elem);
}

void TSet::Clean()
{
  bitField = TBitField(this->GetMaxPower());
}

void TSet::InsElem(const int elem) // включение элемента множества
{
  bitField.SetBit(elem);
}

void TSet::DelElem(const int elem) // исключение элемента множества
{
  bitField.ClrBit(elem);
}
// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
  if (*this == s)
    return *this;
  bitField = s.bitField;
  return *this;
}

bool TSet::operator==(const TSet& s) const // сравнение
{
  return bitField == s.bitField;
}

bool TSet::operator!=(const TSet& s) const // сравнение
{
  return bitField != s.bitField;
}

TSet TSet::operator+(const TSet& s) // объединение
{
  return bitField | s.bitField;
}

TSet TSet::operator+(const int elem) // объединение с элементом
{
  TSet set(*this);
  set.InsElem(elem);
  return set;
}

TSet TSet::operator-(const int elem) // разность с элементом
{
  TSet set(*this);
  set.DelElem(elem);
  return set;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
  return bitField & s.bitField;
}

TSet TSet::operator~() // дополнение
{
  return TSet(~bitField);
}

// перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& s) // ввод
{
  std::vector<int> nums;
  int i;
  s.Clean();
  istr >> i;
  while (i != -1)
  {
    s.InsElem(i);
    istr >> i;
  }
  return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s) // вывод
{
  ostr << "{ ";
  for (int i = 0; i < s.GetMaxPower(); ++i)
    if (s.IsMember(i))
      ostr << i << ", ";
  ostr << "}";
  return ostr;
}