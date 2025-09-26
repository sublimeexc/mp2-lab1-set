// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : bitField(mp)
{
    maxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet &s) : bitField(s.bitField)
{
    maxPower = s.maxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : bitField(bf)
{
    maxPower = bf.GetLength();
}

TSet::operator TBitField()
{
    return bitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return maxPower;
}

int TSet::IsMember(const int n) const // элемент множества?
{
    if (n < 0 || n >= maxPower) 
    {
        throw std::out_of_range("ИСКЛЮЧЕНИЕ: индекс вне множества");
    }
    return bitField.GetBit(n);
}

void TSet::InsElem(const int n) // включение элемента множества
{
    if (n < 0 || n >= maxPower) 
    {
        throw std::out_of_range("ИСКЛЮЧЕНИЕ: индекс вне множества");
    }
    bitField.SetBit(n);
}

void TSet::DelElem(const int n) // исключение элемента множества
{
    if (n < 0 || n >= maxPower) 
    {
        throw std::out_of_range("ИСКЛЮЧЕНИЕ: индекс вне множества");
    }
    bitField.ClrBit(n);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    if (this != &s) 
    {
        maxPower = s.maxPower;
        bitField = s.bitField;
    }
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    if ((maxPower == s.maxPower) && (bitField == s.bitField)) return 1;
    else return 0;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet &s) // объединение
{
    int newmaxPower = std::max(maxPower,s.maxPower);
    TSet result(newmaxPower);
    result.bitField = bitField | s.bitField;
    return result;
}

TSet TSet::operator+(const int n) // объединение с элементом
{
    if (n < 0 || n >= maxPower) 
    {
        throw std::out_of_range("ИСКЛЮЧЕНИЕ: индекс вне множества");
    }
    TSet result(*this);
    result.InsElem(n);
    return result;
}

TSet TSet::operator-(const int n) // разность с элементом
{
    if (n < 0 || n >= maxPower) 
    {
        throw std::out_of_range("ИСКЛЮЧЕНИЕ: индекс вне множества");
    }
    TSet result(*this);
    result.DelElem(n);
    return result;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    int newmaxPower = std::max(maxPower, s.maxPower);
    TSet result(newmaxPower);
    result.bitField = bitField & s.bitField;
    return result;
}

TSet TSet::operator~(void) // дополнение
{
    TSet result(maxPower);
    result.bitField = ~bitField;
    return result;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    int mp;
    istr >> mp;
    if (mp != s.maxPower) 
    {
        s = TSet(mp);
    }
    istr >> s.bitField;
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    for (int i = 0; i < s.GetMaxPower(); i++) 
    {
        if (s.IsMember(i)) 
        {
            ostr << i << " ";
        }
    }
    return ostr;
}
