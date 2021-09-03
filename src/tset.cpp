// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : MaxPower(mp), BitField(mp)
{
}

// конструктор копирования
TSet::TSet(const TSet &s) : MaxPower(s.MaxPower), BitField(s.BitField)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf)
{
	MaxPower = bf.GetLength();
}

TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
	BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
	BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) = default; // присваивание

int TSet::operator==(const TSet &s) const // сравнение
{
    return MaxPower == s.MaxPower && BitField == s.BitField;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet &s) // объединение
{
	TSet res = *this;
	res.BitField = res.BitField | s.BitField;
	res.MaxPower = res.BitField.GetLength();
    return res;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
	if (Elem >= MaxPower)
	{
		throw std::runtime_error("Elem greater than MaxPower");
	}

	TSet a = *this;
	TSet b(Elem + 1);
	b.InsElem(Elem);
	return a.BitField | b.BitField;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
	TSet a = *this;
	a.DelElem(Elem);
	return a;
}

TSet TSet::operator*(const TSet &b) // пересечение
{
	TSet a = *this;
	return a.BitField & b.BitField;
}

TSet TSet::operator~(void) // дополнение
{
    return ~BitField;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    return istr >> s.BitField;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    return ostr << s.BitField;
}
