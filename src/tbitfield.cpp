// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
    if (len < 0) 
    {
        throw std::invalid_argument("ИСКЛЮЧЕНИЕ: отрицательное число"); 
    }
    bitLen = len;
    memLen = std::floor((len + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8));
    pMem = new TELEM[memLen];
    for (int i = 0; i < memLen; i++) 
    {
        pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    bitLen = bf.bitLen;
    memLen = bf.memLen;
    pMem = new TELEM[memLen];
    for (int i = 0; i < memLen; i++) 
    {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    if (pMem != nullptr)
    {
        delete[] pMem;
    }
    pMem = nullptr;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << (n % (sizeof(TELEM) * 8));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return bitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= bitLen) 
    {
        throw std::out_of_range("ИСКЛЮЧЕНИЕ: индекс вне битового поля");
    }
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= bitLen) 
    {
        throw std::out_of_range("ИСКЛЮЧЕНИЕ: индекс вне битового поля");
    }
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= bitLen) 
    {
        throw std::out_of_range("ИСКЛЮЧЕНИЕ: индекс вне битового поля");
    }
    return (pMem[GetMemIndex(n)] & GetMemMask(n)) != 0;
}
// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this != &bf) 
    {
        if (pMem != nullptr)
        {
            delete[] pMem;
        }
        bitLen = bf.bitLen;
        if (memLen != bf.memLen)
        {
            memLen = bf.memLen; // деструктор
        }
        pMem = new TELEM[memLen];
        for (int i = 0; i < memLen; i++) 
        {
            pMem[i] = bf.pMem[i]; // конструктор копирования
        }
    }
    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (bitLen != bf.bitLen) 
    {
        return 0;
    }
    for (int i = 0; i < memLen; i++) 
    {
        if (pMem[i] != bf.pMem[i]) 
        {
            return 0;
        }
    }
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    int MaxLen = std::max(bitLen, bf.bitLen);
    TBitField result(MaxLen);

    for (int i = 0; i < memLen; i++) 
    {
        result.pMem[i] = pMem[i];
    }
    for (int i = 0; i < bf.memLen; i++) 
    {
        result.pMem[i] |= bf.pMem[i];
    }
    return result;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    int maxLen = std::max(bitLen, bf.bitLen);
    TBitField result(maxLen);

    for (int i = 0; i < result.memLen; i++) 
    {
        result.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return result;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField result(bitLen);
    for (int i = 0; i < memLen; i++) 
    {
        result.pMem[i] = ~pMem[i];
    }

    // Очищаем лишние биты в последнем элементе
    int extraBits = bitLen % (sizeof(TELEM) * 8);
    if (extraBits > 0) 
    {
        TELEM mask = (1 << extraBits) - 1;
        result.pMem[memLen - 1] &= mask;
    }
    return result;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    for (int i = 0; i < bf.bitLen; i++) 
    {
        int bit;
        istr >> bit;
        if (bit == 1) 
        {
            bf.SetBit(i);
        }
        else 
        {
            bf.ClrBit(i);
        }
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    for (int i = 0; i < bf.bitLen; i++) 
    {
        ostr << bf.GetBit(i);
    }
    return ostr;
}
