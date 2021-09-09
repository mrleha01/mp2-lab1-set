// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
	: pMem(nullptr)
{
	if (len < 0)
	{
		throw std::runtime_error("create bitfield with negative length");
	}

	BitLen = len;
	if (BitLen == 0)
	{
		MemLen = 0;
		return;
	}

	MemLen = (BitLen - 1) / 32 + 1;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; ++i)
	{
		pMem[i] = 0;
	}
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
	: TBitField(bf.BitLen)
{
	if (pMem)
	{
		for (int i = 0; i < MemLen; ++i)
		{
			pMem[i] = bf.pMem[i];
		}
	}
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n < 0)
		return FAKE_INT;
	return n / 32;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	/*if (pMem == nullptr || n < 0)
		return FAKE_INT;
	TELEM byte = pMem[GetMemIndex(n)];*/
	int bitPos = n % 32;
	return 1 << bitPos;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0)
	{
		throw std::runtime_error("set bit with negative index");
	}
	if (pMem == nullptr || n >= BitLen)
	{
		throw std::runtime_error("set bit with too large index");
	}
	TELEM& byte = pMem[GetMemIndex(n)];
	byte |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0)
	{
		throw std::runtime_error("clear bit with negative index");
	}
	if (pMem == nullptr || n >= BitLen)
	{
		throw std::runtime_error("clear bit with too large index");
	}
	TELEM& byte = pMem[GetMemIndex(n)];
	byte &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0)
	{
		throw std::runtime_error("get bit with negative index");
	}
	if (pMem == nullptr || n >= BitLen)
	{
		throw std::runtime_error("get bit with too large index");
	}
	int memIndex = GetMemIndex(n);
	TELEM byte = pMem[memIndex];
	byte &= GetMemMask(n);
	return byte != 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this == &bf)
		return *this;
	BitLen = bf.BitLen;
	if (MemLen != bf.MemLen)
	{
		MemLen = bf.MemLen;
		delete[] pMem;
		pMem = new TELEM[MemLen];
	}
	for (int i = 0; i < MemLen; ++i)
	{
		pMem[i] = bf.pMem[i];
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (pMem == nullptr && bf.pMem == nullptr)
		return true;
	if (pMem == nullptr || bf.pMem == nullptr)
		return false;
	if (BitLen != bf.BitLen)
		return false;
	for (int i = 0; i < MemLen; ++i)
	{
		if (pMem[i] != bf.pMem[i])
			return false;
	}
	return true;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	if (pMem == nullptr && bf.pMem == nullptr)
		return *this;
	if (pMem == nullptr)
		return bf;
	if (bf.pMem == nullptr)
		return *this;
	TBitField maxBitField = (BitLen > bf.BitLen ? *this : bf); // битовое поле с большей длиной. Используется для хранения результата
	const TBitField& minBitField = (BitLen <= bf.BitLen ? *this : bf); // битовое поле с меньшей длиной.
	for (int i = 0; i < MemLen && i < bf.MemLen; ++i)
	{
		maxBitField.pMem[i] |= minBitField.pMem[i];
	}
	return maxBitField;
    //return FAKE_BITFIELD;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	if (pMem == nullptr && bf.pMem == nullptr)
		return *this;
	if (pMem == nullptr)
		return bf;
	if (bf.pMem == nullptr)
		return *this;
	TBitField maxBitField = (BitLen > bf.BitLen ? *this : bf); // битовое поле с большей длиной. Используется для хранения результата
	const TBitField& minBitField = (BitLen <= bf.BitLen ? *this : bf); // битовое поле с меньшей длиной.
	for (int i = 0; i < MemLen && i < bf.MemLen; ++i)
	{
		maxBitField.pMem[i] &= minBitField.pMem[i];
	}
	return maxBitField;
	//return FAKE_BITFIELD;
}

TBitField TBitField::operator~(void) // отрицание
{
	if (pMem == nullptr)
		return FAKE_BITFIELD;
	TBitField res(*this);
	for (int i = 0; i < MemLen - 1; ++i)
	{
		res.pMem[i] = ~res.pMem[i];
	}
	// invert last byte
	TELEM& lastByte = res.pMem[MemLen - 1];
	int numOfBitInLastByte = BitLen % 32;
	int shift = (MemLen - 1) * 32;
	for (int i = 0; i < numOfBitInLastByte; ++i)
	{
		int bitIndex = shift + i;
		int bit = res.GetBit(bitIndex);
		if (bit == 0)
		{
			res.SetBit(bitIndex);
		}
		else if (bit == 1)
		{
			res.ClrBit(bitIndex);
		}
	}
	return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	char c = istr.get();
	for (int i = 0, c = istr.get(); c != '\n'; ++i, c = istr.get())
	{
		if (c == '0')
		{
			bf.ClrBit(i);
		}
		else if (c == '1')
		{
			bf.SetBit(i);
		}
	}
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	if (bf.pMem != nullptr)
	{
		for (int i = 0; i < bf.BitLen; ++i)
		{
			ostr << bf.GetBit(i);
		}
	}
    return ostr;
}
