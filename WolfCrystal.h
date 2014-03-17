/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * WolfCrystals v1.0.0
 * ����������, ��������������� ��� ����������� ��������� �������� ����� 
 * ���� �� HD44780-����������� ��������, � ������ ��������������� ������� 
 * ���������� ���� ������ ��������, � �� ���������.
 *
 * ��� �������������� ���� ������ ������� ���� ��������� ��
 * ��������� �� ����������� �������� � ��������. ��� ���� ������� ����
 * ���� ���������� ����������� �� �����. 
 * 
 * ���������� ���������� � Arduino IDE 1.0+
 * (C) 2014 Ivan Klenov, Wolf4D@list.ru
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 */

#ifndef WOLFCRYSTALS_H
#define WOLFCRYSTALS_H

#include <inttypes.h>
#include <LiquidCrystal.h>

class WolfCrystal
{
public:

// ����������� � ���������� LCD
WolfCrystal(LiquidCrystal * LCD=0);
void AddLCD(LiquidCrystal * LCD);

// �������(�) �������������� ������ � ���, ��������� ��� �������� �� �������.
String GS(char *input);
String GS(String input);

private:
// ProcessChars ������������ �������� ������.
String ProcessChars(String input);

// ���������� ����� � ������ �����������.
void CreateCapsGlyphs();

// ������ �� ������ LiquidCrystal
LiquidCrystal * myLCD;

};

#endif // WOLFCRYSTALS_H