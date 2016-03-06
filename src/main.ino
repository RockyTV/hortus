/*
 * Copyright (C) 2016 Alexandre Oliveira
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Arduino.h>
#include <TM1637Display.h>

int realizarLeituras();
void limparDisplay();

#define SENSOR A5
#define VCC 3
#define BUZZER 4

#define CLK 8
#define DIO 9

TM1637Display display(CLK, DIO);

unsigned long tempoLeitura;
unsigned long sensorLigado;
unsigned long tempoLed;
unsigned long tempoBuzzer;

const long intervalo = 5000;
const long intervaloBuzzer = 250;
const long INTERVALO_LIGAR = 10000;

int estadoBuzzer = LOW;
int ultimaLeitura = -1;

bool ligarLeds = true;
bool iniciou = false;

void limparDisplay() {
    const uint8_t LIMPAR[] = {
        0x00,
        0x00,
        0x00,
        0x00
    };
    display.setSegments(LIMPAR);
}

void setup() {
    pinMode(SENSOR, INPUT);
    pinMode(VCC, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    display.setBrightness(100);
    display.showNumberDec(0, false, 4, 0);
    iniciou = true;
    limparDisplay();
}

int realizarLeituras() {
    // Ligar o sensor
    digitalWrite(VCC, HIGH);

    int leitura = 1100;

    for (int i = 0; i < 10; i++) {
        leitura = analogRead(SENSOR);
        leitura = map(leitura, 0, 1023, 100, 0);
    }

    digitalWrite(VCC, LOW);
    return leitura;
}

const uint8_t AGUA[] = {
    SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,
    SEG_A | SEG_G | SEG_E | SEG_F | SEG_C | SEG_D,
    SEG_F | SEG_E | SEG_D | SEG_C | SEG_B,
    SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G
};

const uint8_t ERRO[] = {
    SEG_A | SEG_F | SEG_G | SEG_E | SEG_D,
    SEG_E | SEG_G,
    SEG_E | SEG_G,
    SEG_E | SEG_D | SEG_C | SEG_G
};

void loop() {
    if (ligarLeds) {
        for (int i = 0; i < 5; i++) {
            digitalWrite(BUZZER, HIGH);
            delay(50);
            digitalWrite(BUZZER, LOW);
            delay(50);
        }
        ligarLeds = false;
    }

    unsigned long tempoAtual = millis();
    // Esperar x tempo para realizar as leituras
    if (tempoAtual - tempoLeitura >= intervalo || iniciou) {
        int leitura = realizarLeituras();
        ultimaLeitura = leitura;
        tempoLeitura = tempoAtual;

        if (iniciou)
            iniciou = false;
    }

    // 1100 quer dizer que a leitura não foi realizada
    if (ultimaLeitura == 1100) {
        display.setSegments(ERRO);
    }

    if (ultimaLeitura != -1) {
        // Sensor desconectado ou solo muito seco
        if (ultimaLeitura < 25) {
            if (tempoAtual - tempoBuzzer >= intervaloBuzzer) {
                tempoBuzzer = tempoAtual;
                (estadoBuzzer == LOW ? estadoBuzzer = HIGH : estadoBuzzer = LOW);
                digitalWrite(BUZZER, estadoBuzzer);
            }
        }

        ultimaLeitura >= 25 ? display.showNumberDec(ultimaLeitura, false, 4, 4) : display.setSegments(AGUA);

        // Solo está SECO
        if (ultimaLeitura < 40 && ultimaLeitura >= 25) {
            if (tempoAtual - tempoBuzzer >= 500) {
                tempoBuzzer = tempoAtual;
                if (estadoBuzzer == LOW)
                    estadoBuzzer = HIGH;
                else
                    estadoBuzzer = LOW;

                digitalWrite(BUZZER, estadoBuzzer);
            }
        }
    }
}
