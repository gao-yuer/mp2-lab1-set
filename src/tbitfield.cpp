// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField()
{
  bitLen = 0;
  memLen = 0;
  pMem = nullptr;
}

TBitField::TBitField(int bitLen_, TELEM* pMem_, int memLen_)
{
  bitLen = bitLen_;
  pMem = pMem_;
  memLen = memLen_;
}

TBitField::TBitField(int len)
{
  if (len <= 0)
    throw invalid_argument("Длина битового поля должна быть положительной");

  // Вычисляем необходимое количество элементов памяти
  bitLen = len;
  memLen = (len + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8); 
  pMem = new TELEM[memLen];

  // Инициализируем память нулями
  for (int i = 0; i < memLen; i++)
    pMem[i] = 0;
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
  if (bf.pMem)
  {
    bitLen = bf.bitLen;
    memLen = bf.memLen;
    pMem = new TELEM[memLen];

    for (int i = 0; i < memLen; i++)
      pMem[i] = bf.pMem[i];
  }
  else
  {
    bitLen = 0;
    memLen = 0;
    pMem = nullptr;
  }
  
}

TBitField::~TBitField()
{
  if(pMem)
  {
    delete[] pMem;
    pMem = nullptr;
    bitLen = 0;
    memLen = 0;
  }
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
  if(n < 0 || n >= bitLen)
    throw out_of_range("Бит за пределами диапазона");

  return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
  if (n < 0 || n >= bitLen)
    throw out_of_range("Бит за пределами диапазона");

  return 1 << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return bitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
  int index = GetMemIndex(n);
  TELEM mask = GetMemMask(n);
  pMem[index] |= mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
  int index = GetMemIndex(n);
  TELEM mask = GetMemMask(n);
  pMem[index] &= ~mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
  int index = GetMemIndex(n);
  TELEM mask = GetMemMask(n);
  return (pMem[index] & mask) != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
  if (this != &bf) // Защита от самоприсваивания
  {
    delete[] pMem;
    bitLen = bf.bitLen;
    memLen = bf.memLen;
    pMem = new TELEM[memLen];
    for (int i = 0; i < memLen; i++)
      pMem[i] = bf.pMem[i];
  }
  return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
  if (bitLen != bf.bitLen)
    return 0;

  for (int i = 0; i < memLen; i++)
    if (pMem[i] != bf.pMem[i])
      return 0;

  return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
  int maxLen = max(bitLen, bf.bitLen); 
  TBitField result(maxLen);  

  for (int i = 0; i < min(memLen, result.memLen); i++)  // Копирование текущего поля
    result.pMem[i] = pMem[i];  // (только общие элементы)

  for (int i = 0; i < min(bf.memLen, result.memLen); i++)  // Применение операции ИЛИ
    result.pMem[i] |= bf.pMem[i];  // Побитовое ИЛИ с вторым полем

  return result; 
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
  int maxLen = max(bitLen, bf.bitLen);  // Берем максимальную длину
  TBitField result(maxLen);  // Создаем результат максимальной длины

  // Применяем операцию И только к общей части
  int minMemLen = min(memLen, bf.memLen);
  for (int i = 0; i < minMemLen; i++)
  {
    result.pMem[i] = pMem[i] & bf.pMem[i];
  }

  return result;
}

TBitField TBitField::operator~(void) // отрицание
{
  TBitField result(bitLen); 

  for (int i = 0; i < memLen; i++)  
    result.pMem[i] = ~pMem[i];  

  if (memLen > 0)  
  {
    int extraBits = bitLen % (sizeof(TELEM) * 8);  // Вычисление лишних битов в последнем элементе
    if (extraBits > 0)  
    {
      TELEM mask = (1 << extraBits) - 1;  // Создание маски для значимых битов
      result.pMem[memLen - 1] &= mask;  // Обнуление неиспользуемых битов
    }
  }

  return result;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
  for (int i = 0; i < bf.bitLen; i++)  
  {
    int bit;
    istr >> bit; 
    if (bit == 1)  
      bf.SetBit(i); 
    else if (bit == 0)  
      bf.ClrBit(i);  
    else  
      throw invalid_argument("Некорректное значение бита");  
  }
  return istr; 
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
  for (int i = 0; i < bf.bitLen; i++)  
    ostr << bf.GetBit(i) << " ";  
  return ostr;  
}
