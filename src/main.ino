#include <LiquidCrystal.h>     /* Library untuk modul lcd arduino */
#include <OneWire.h>           /* Library pendukuk sensor DS18b20 */
#include <DallasTemperature.h> /* Library untuk sensor DS18b20 */

int led_hijau = 2;      /* positif led hijau */
int led_merah = 3;      /* positif led merah */
int relay_pin = 4;      /* positif relay */
int ds28b20_data = 7;   /* ds28b20 data pin */

int lcd_rs = 8;         /* lcd rs pin */
int lcd_en = 9;         /* lcd en pin */
int lcd_d4 = 10;        /* lcd d4 pin */
int lcd_d5 = 11;        /* lcd d5 pin */
int lcd_d6 = 12;        /* lcd d6 pin */
int lcd_d7 = 13;        /* lcd d7 pin */

float temperature_c;
float temperature_f;

/* Inisialisasi lcd */
LiquidCrystal lcd(lcd_rs, lcd_en, lcd_d4, lcd_d5, lcd_d6, lcd_d7);
/* Inisialisasi sensor DS18b20 */
OneWire oneWire(ds28b20_data);
DallasTemperature sensor(&oneWire);

void setup() {
    /* Aktifkan lcd */
    lcd.begin(16, 2);
    /* Aktifkan sensor DS18b20 */
    sensor.begin();
    /* Aktifkan serial monitor */
    Serial.begin(9600);
    Serial.println("Mamulai alat pengontrol suhu air akuarium");

    /* Setting pin-pin sebagai output */
    pinMode(relay_pin, OUTPUT);
    pinMode(led_hijau, OUTPUT);
    pinMode(led_merah, OUTPUT);

    /* Matikan semua pin */
    digitalWrite(relay_pin, LOW);
    digitalWrite(led_hijau, LOW);
    digitalWrite(led_merah, LOW);
}

void loop() {
    /* Baca temperatur air pada akuarium */
    sensor.requestTemperatures();
    /* Konversi temperatur ke Celcius */
    temperature_c = sensor.getTempCByIndex(0);
    /* Konversi temperatur ke Fahrenheit */
    temperature_f = sensor.getTempFByIndex(0);

    /* Tampilkan temperatur pada serial monitor */
    Serial.print("Temperature: ");
    Serial.print(temperature_c);
    Serial.print(" ^C, ");
    Serial.print(temperature_f);
    Serial.print(" ^F, ");


    /* Refresh lcd */
    lcd.clear();
    /* Print pada baris pertama lcd */
    lcd.setCursor(0, 0);
    /* Tampilkan temperatur pada lcd */
    lcd.print("Temp:");
    lcd.print(temperature_c, 0);
    /* Simbol derajat */
    lcd.write(0xDF);
    lcd.print("C ");
    lcd.print(temperature_f, 0);
    /* Simbol derajat */
    lcd.write(0xDF);
    lcd.print("F");
    /* Print pada baris kedua lcd */
    lcd.setCursor(0, 1);

    /* Jika temperatur dibawah 28^C */
    if (temperature_c <= 28.0) {
        /* Hidupkan pemanas air */
        digitalWrite(relay_pin, HIGH);
        /* Matikan led hijau */
        digitalWrite(led_hijau, LOW);
        /* Hidupkan led merah */
        digitalWrite(led_merah, HIGH);
        /* Tampilkan status air dan pemanas ke lcd */
        lcd.print("Air:Dng Htr:");
        lcd.print(digitalRead(relay_pin) ? "On" : "Off");
        /* Tampilkan status air dan pemanas ke serial monitor */
        Serial.print("Keadaan air: Dingin, Pemanas: ");
        Serial.println(digitalRead(relay_pin) ? "Hidup" : "Mati");
    }
    /* Jika temperatur diantara 28.0 dan 30.0 Celcius */
    else if (temperature_c > 28.0 && temperature_c < 30.0) {
        /* Matikan pemanas air */
        digitalWrite(relay_pin, LOW);
        /* Hidupkan led hijau */
        digitalWrite(led_hijau, HIGH);
        /* Matikan led merah */
        digitalWrite(led_merah, LOW);
        /* Tampilkan status air dan pemanas ke lcd */
        lcd.print("Air:Nrm Htr:");
        lcd.print(digitalRead(relay_pin) ? "On" : "Off");
        /* Tampilkan status air dan pemanas ke serial monitor */
        Serial.print("Keadaan air: Normal, Pamanas: ");
        Serial.println(digitalRead(relay_pin) ? "Hidup" : "Mati");
    }
    /* Jika temperatur diatas 30^C */
    else if (temperature_c >= 30.0) {
        /* Matikan pemanas air */
        digitalWrite(relay_pin, LOW);
        /* Hidupkan led hijau */
        digitalWrite(led_hijau, HIGH);
        /* Matikan led merah */
        digitalWrite(led_merah, LOW);
        /* Tampilkan status air dan pemanas ke lcd */
        lcd.print("Air:Pns Htr:");
        lcd.print(digitalRead(relay_pin) ? "On" : "Off");
        /* Tampilkan status air dan pemanas ke serial monitor */
        Serial.print("Keadaan air: Normal, Pamanas: ");
        Serial.println(digitalRead(relay_pin) ? "Hidup" : "Mati");
    }
    /* Jeda selama 300ms */
    delay(300);
}
