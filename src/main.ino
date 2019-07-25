#include <LiquidCrystal.h>     /* Library untuk modul lcd arduino */
#include <OneWire.h>           /* Library pendukuk sensor DS18b20 */
#include <DallasTemperature.h> /* Library untuk sensor DS18b20 */

int led_hijau = 3;     /* positif led hijau */
int led_merah = 4;     /* positif led merah */
int ds28b20_data = 5;  /* ds28b20 data pin */

int lcd_d4 = 6;        /* lcd d4 pin */
int lcd_d5 = 7;        /* lcd d5 pin */
int lcd_d6 = 8;        /* lcd d6 pin */
int lcd_d7 = 9;        /* lcd d7 pin */
int lcd_rs = 10;       /* lcd rs pin */
int lcd_en = 11;       /* lcd en pin */

int relay_pin = 12;    /* positif relay */

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
    temperature_c = sensor.getTempCByIndex(0);
    temperature_f = sensor.getTempFByIndex(0);

    /* Tampilkan temperatur pada serial monitor */
    Serial.print("Temperature: ");
    Serial.print(temperature_c);
    Serial.print(" ^C, ");
    Serial.print(temperature_f);
    Serial.print(" ^F, ");

    /* Tampilkan temperatur pada lcd */
    lcd.print("Temp:");
    lcd.print(temperature_c, 0);
    lcd.print("^C ");
    lcd.print(temperature_f, 0);
    lcd.println("^F");

    /* Jika temperatur dibawah 28^C */
    if (temperature_c <= 28.0) {
        digitalWrite(relay_pin, HIGH);
        digitalWrite(led_hijau, LOW);
        digitalWrite(led_merah, HIGH);

        /* Tampilkan status air dan relay */
        lcd.print("Air:Dng ");
        lcd.print("Rly:");
        lcd.print(digitalRead(relay_pin) ? "On" : "Off");

        Serial.print("Keadaan air: Dingin, Relay: ");
        Serial.println(digitalRead(relay_pin) ? "Hidup" : "Mati");

    }
    /* Jika temperatur diatas 30^C */
    else if (temperature_c >= 30.0) {
        digitalWrite(relay_pin, LOW);
        digitalWrite(led_hijau, HIGH);
        digitalWrite(led_merah, LOW);

        /* Tampilkan status air dan relay */
        lcd.print("Air:Pns ");
        lcd.print("Rly:");
        lcd.print(digitalRead(relay_pin) ? "On" : "Off");

        Serial.print("Keadaan air: Normal, Relay: ");
        Serial.println(digitalRead(relay_pin) ? "Hidup" : "Mati");

    }

    delay(100);
    /* Refresh lcd */
    lcd.clear();
}
