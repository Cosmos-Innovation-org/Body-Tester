# Body Tester — Birinchi O‘zbek Robo-Doktor 🤖🇺🇿

**Body Tester** — bu birinchi to‘liq o‘zbek tilida ishlovchi, ko‘p funksiyali avtomatlashtirilgan **robo-doktor** bo‘lib, bir vaqtning o‘zida **6 ta tibbiy ko‘rsatkichni** o‘lchay oladi. Qurilma zamonaviy Arduino texnologiyasi va kuchli sensorlar asosida ishlab chiqilgan bo‘lib, sog‘liqni monitoring qilish jarayonini 100% avtomatlashtirishga qaratilgan.

## 🔬 O‘lchash funksiyalari

Qurilma quyidagi sog‘liq holatini baholovchi funksiyalarni bajaradi:

- 📏 **Bo‘y** (207 sm gacha)
- ⚖️ **Vazn** (300 kg gacha)
- ❤️ **Yurak urish tezligi (puls)**
- 🌬️ **Qondagi kislorod miqdori (SpO2)**
- 💉 **Qon bosimi (NIBP UN300C moduli orqali)**
- 🌡️ **Tana harorati (DS18B20 orqali)**

## 📊 Intellektual hisob-kitob

Body Tester quyidagilarni dasturiy ta'minot orqali hisoblab beradi:

- **Tana vazn indeksi (BMI)** — foydalanuvchining yoshi va bo‘yiga qarab kerakli vazn ko‘rsatkichlarini aniqlaydi.
- **Normal ko‘rsatkichlar oralig‘i** — foydalanuvchining holatini vizual baholash.
- **Statistik tahlil** — foydalanuvchi tarixiy natijalarini saqlash va solishtirish.

## 👤 Shaxsiy kabinet & bazaga ulanish

- Har bir foydalanuvchiga shaxsiy kabinet yaratiladi.
- Ma’lumotlar doimiy bazaga yoziladi va keyinchalik tahlil qilish imkonini beradi.
- Tizim ichki ID bilan har bir odamni tanib oladi.

## 🗣 Til va interfeys

- Qurilma **to‘liq o‘zbek tilida** ishlaydi.
- Sensorli **10.1 dyuymli, 1024x600 RGB UART displey** orqali boshqariladi.
- Har bir amal davomida **audio tizim** orqali o‘zbek tilida ogohlantirishlar beriladi.

## ⏱ Ishlash tezligi

- Qurilma barcha funksiyalarni **1.5 daqiqa** ichida bajaradi.
- Istalgan funksiyani alohida tanlab o‘lchash imkoniyati mavjud.
- Barcha o‘lchovlar **bir vaqtning o‘zida** yoki foydalanuvchi tanlovi asosida amalga oshiriladi.

## 📦 Qurilma texnik tavsifi

| Parametr                  | Qiymat                        |
|---------------------------|-------------------------------|
| O‘lcham                   | 222 x 47 x 60 sm              |
| Korpus materiali          | Poliferniy smala              |
| Displey                   | 10.1" sensorli, 1024x600 RGB  |
| Maks. bo‘y o‘lchovi       | 207 sm                        |
| Maks. vazn o‘lchovi       | 300 kg                        |
| Qon bosimi sensori        | NIBP UN300C                   |
| Ish vaqti                 | 1.5 daqiqa                    |
| Til                       | O‘zbek tili (audio + vizual) |

## 🧠 Dasturiy ta'minot haqida

- **Platforma:** Arduino IDE (C++ asosida)
- **Fayllar:** `.ino` (asosiy kod), `.h` (grafika/logotip)
- **Kutubxonalar:** `Adafruit_GFX`, `SSD1306`, `DallasTemperature`, `OneWire` va boshqalar

## 🚀 Ishga tushirish bo‘yicha qisqacha ko‘rsatma

1. Arduino IDE'ni ishga tushiring.
2. `body_tester_Namanganv.ino` va `adalogo.h` fayllarini oching.
3. Kerakli kutubxonalarni o‘rnating.
4. Qurilmani USB orqali kompyuterga ulang.
5. Portni tanlab, kodni yuklang.
6. Qurilma ishga tushadi va foydalanuvchini kutish rejimida bo‘ladi.

## 🎯 Loyiha maqsadi

> "Tibbiyot sohasini 100% raqamlashtirish va sog‘lom turmush tarzini aholi orasida keng targ‘ib qilish."

**Body Tester** nafaqat tibbiyot muassasalari uchun, balki maktablar, sport zallari va keng jamoatchilik foydalanishi uchun mo‘ljallangan.

---

🤝 **Muallif(lar): Cosmos Innovation
🏆 **Tanlov:** President Tech Award 2025
📅 **Versiya:** 1.0  
🌐 **Til:** O‘zbekcha  
