/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * WolfCrystals v1.0.0
 * Библиотека, переделана  для обеспечения поддержки русского языка 
 * на WH1602B-TMI-CT# - дисплее. (Азбука электронщика от masterkit)
 *
 * Это осуществляется путём замены русских букв подобными им
 * символами из английского алфавита. Для семи русских букв
 * были нарисованы имитирующие их глифы. 
 * 
 * Библиотека тестировалась с Arduino IDE 1.8.0
 * (C) 2014 Ivan Klenov, Wolf4D@list.ru
 * 2017 правил Илья, grayrat@ya.ru
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

#include "WolfCrystal.h"
#include <LiquidCrystal.h>
#include <Wstring.h>


#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

///////////////////////////////////////////////////////////////////////////////
// Для большого количества строчных русских символов эквиваленты выглядели 
// гораздо хуже, чем для прописных (заглавных). 

// Потому по умолчанию все строчные русские буквы переводятся в прописные.
// Если требуется включить вывод русских строчных (а они местами ой как корявы),
// то раскомментируйте следующую строку:
#define CASE_SENSITIVE
///////////////////////////////////////////////////////////////////////////////

// Глифы русских букв (использовано из них 7 штук, 1 глиф свободен):
byte be[8] = {
  0b11111,
  0b10000,
  0b10000,
  0b11110,
  0b10001,
  0b10001,
  0b11110,
  0b00000
};

byte ge[8] = {
  0b11111,
  0b10001,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b10000,
  0b00000
};


byte de[8] = {
  0b00111,
  0b01001,
  0b01001,
  0b01001,
  0b01001,
  0b01001,
  0b01111,
  0b10001
};

byte zje[8] = {
  0b10101,
  0b10101,
  0b01110,
  0b00100,
  0b01110,
  0b10101,
  0b10101,
  0b00000
};


byte pe[8] = {
  0b11111,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b00000
};

byte ii[8] = {
  0b10001,
  0b10011,
  0b10101,
  0b10101,
  0b10101,
  0b11001,
  0b10001,
  0b00000
};

byte uu[8] = {
  0b10010,
  0b10101,
  0b10101,
  0b11101,
  0b10101,
  0b10101,
  0b10010,
  0b00000
};


byte ll[8] = {
  0b00111,
  0b01001,
  0b01001,
  0b01001,
  0b01001,
  0b01001,
  0b10001,
  0b00000
};


byte ce[8] = {
  0b10010,
  0b10010,
  0b10010,
  0b10010,
  0b10010,
  0b10010,
  0b11111,
  0b00001
};

///////////////////////////////////////////////////////////////////////////////
// Конструктор и добавление LCD
WolfCrystal::WolfCrystal(LiquidCrystal * LCD) 
{ AddLCD(LCD); };

void WolfCrystal::AddLCD(LiquidCrystal * LCD)
{
if (LCD!=0) 
{myLCD=LCD; CreateCapsGlyphs();}
};

///////////////////////////////////////////////////////////////////////////////
// Функция преобразования строки в вид, пригодный для отправки на дисплей.

// Оформляет строку единообразно, вне зависимости от типа вызова (с char* или
// со String, и передаёт её в ProcessChars.
String WolfCrystal::GS(char *input)
{
String answer=String(input);
return ProcessChars(answer);
};

String WolfCrystal::GS(String input)
{return ProcessChars(input);};

///////////////////////////////////////////////////////////////////////////////
// ProcessChars обрабатывает введённую строку.
// Мне не удалось придумать лёгкого способа замены символов проще и эффективнее,
// чем обычный case. Тем более, что некоторые символы заменяются на  
// два. Кроме того, Arduino имеет несколько изъянов:

String WolfCrystal::ProcessChars(String input)
{
/*
Для хранения русского языка Arduino использует двухбайтовую кодировку.
Функция toUpperCase не работает с русским в таком виде. Похоже, она вообще 
ни в каком виде с национальными языками не работает - впрочем, на сайте
разработчиков это (весьма неявно) сказано.
Пришлось прописывать таблицу соответствий без оптимизаций, вручную.

Плюс, двухбайтовость кодировки даёт о себе знать при разложении String
в массив с использованием стандартного toCharArray. String копируется в 
массив _побайтно_, несмотря на то, что символы 2байтовые. 
То есть на русскую букву в массив char-ов пишется два байта. 

НО!
При этом преобразование из двухбайтовой в однобайтовую кодировку совершается!
Корректно преобразованный символ пишется в первый выделенный под символ char,
а во второй char попадает мусор. По этой причине вылезают "левые" символы, 
находящиеся в заведомо неиспользуемых областях кодовых таблиц. 

Можно было бы отбросить каждый второй char, но тогда могут пострадать 
однобайтовые символы. Потому был применён примитивный фильтр, отсекающий
мусор.

Также иногда по странной причине съедаются части строки - потому
было использовано два ни на что толком не влияющих костыля.
*/
String aaa;
input.replace("А","A");
aaa=char(160);input.replace("Б",aaa);
input.replace("В","B");
aaa=char(161);input.replace("Г",aaa);
aaa=char(224);input.replace("Д",aaa);
input.replace("Е","E");
aaa=char(162);input.replace("Ё",aaa);
aaa=char(163);input.replace("Ж",aaa);
aaa=char(164);input.replace("З",aaa);
aaa=char(165);input.replace("И",aaa);
aaa=char(166);input.replace("Й",aaa);
input.replace("К","K");
aaa=char(167);input.replace("Л",aaa);
input.replace("М","M");
input.replace("Н","H");
input.replace("О","O");
aaa=char(168);input.replace("П",aaa);
input.replace("Р","P");
input.replace("С","C");
input.replace("Т","T");
aaa=char(169);input.replace("У",aaa);
aaa=char(170);input.replace("Ф",aaa);
input.replace("Х","X");
aaa=char(225);input.replace("Ц",aaa);
aaa=char(171);input.replace("Ч",aaa);
aaa=char(172);input.replace("Ш",aaa);
aaa=char(226);input.replace("Щ",aaa);
aaa=char(173);input.replace("Ъ",aaa);
aaa=char(174);input.replace("Ы",aaa);
aaa=char(226);input.replace("Ь",aaa);
aaa=char(175);input.replace("Э",aaa);
aaa=char(176);input.replace("Ю",aaa);
aaa=char(177);input.replace("Я",aaa);
// Так как никакие toUpper не работают для кириллицы, то просто прописано
// две ветви выполнения для разных настроек библиотеки. Это позволяет уменьшить
// потребление SRAM.
#ifdef CASE_SENSITIVE
input.replace("а","a");
aaa=char(178);input.replace("б",aaa);
aaa=char(179);input.replace("в",aaa);
aaa=char(180);input.replace("г",aaa);
aaa=char(227);input.replace("д",aaa);
input.replace("е","e");
aaa=char(181);input.replace("ё",aaa);
aaa=char(182);input.replace("ж",aaa);
aaa=char(183);input.replace("з",aaa);
aaa=char(184);input.replace("и",aaa);
aaa=char(185);input.replace("й",aaa);
aaa=char(186);input.replace("к",aaa);
aaa=char(187);input.replace("л",aaa);
aaa=char(188);input.replace("м",aaa);
aaa=char(189);input.replace("н",aaa);
input.replace("о","o");
aaa=char(190);input.replace("п",aaa);
input.replace("р","p");
input.replace("с","c");
aaa=char(191);input.replace("т",aaa);
input.replace("у","y");
aaa=char(228);input.replace("ф",aaa);
input.replace("х","x");
aaa=char(229);input.replace("ц",aaa);
aaa=char(192);input.replace("ч",aaa);
aaa=char(193);input.replace("ш",aaa);
aaa=char(226);input.replace("щ",aaa);
aaa=char(196);input.replace("ь",aaa);
aaa=char(195);input.replace("ы",aaa);
aaa=char(194);input.replace("ъ",aaa);
aaa=char(197);input.replace("э",aaa);
aaa=char(198);input.replace("ю",aaa);
aaa=char(199);input.replace("я",aaa);
#else
input.replace("а","A");
aaa=char(160);input.replace("б",aaa);
input.replace("в","B");
aaa=char(161);input.replace("г",aaa);
aaa=char(224);input.replace("д",aaa);
input.replace("е","E");
aaa=char(162);input.replace("ё",aaa);
aaa=char(163);input.replace("ж",aaa);
aaa=char(164);input.replace("з",aaa);
aaa=char(165);input.replace("и",aaa);
aaa=char(166);input.replace("й",aaa);
input.replace("к","K");
aaa=char(167);input.replace("л",aaa);
input.replace("м","M");
input.replace("н","H");
input.replace("о","O");
aaa=char(168);input.replace("п",aaa);
input.replace("р","P");
input.replace("с","C");
input.replace("т","T");
aaa=char(169);input.replace("у",aaa);
aaa=char(170);input.replace("ф",aaa);
input.replace("х","X");
aaa=char(225);input.replace("ц",aaa);
aaa=char(171);input.replace("ч",aaa);
aaa=char(172);input.replace("ш",aaa);
aaa=char(226);input.replace("щ",aaa);
aaa=char(173);input.replace("ъ",aaa);
aaa=char(174);input.replace("ы",aaa);
aaa=char(226);input.replace("ь",aaa);
aaa=char(175);input.replace("э",aaa);
aaa=char(176);input.replace("ю",aaa);
aaa=char(177);input.replace("я",aaa);
#endif
// Фильтр ввода. Если символ не конвертирован, но не за пределами "значимых" по
// кодовой таблице, то пишем его тоже.
return input;
};

//#endif

// Записываем глифы в память контроллера
void WolfCrystal::CreateCapsGlyphs()
{

  myLCD->createChar(1, be);
  myLCD->createChar(2, ge);
  myLCD->createChar(3, de);  
  myLCD->createChar(4, pe); 
  myLCD->createChar(5, ii);  
  myLCD->createChar(6, ll); 
  myLCD->createChar(7, ce);
};
