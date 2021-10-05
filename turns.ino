#define DEBUG 1

#define LEFT_TURN_RELAY 4 // Пин реле левого поворотника
#define LEFT_TURN_SIGNAL 10 // Пин сигнального провода левого поворотника
#define RIGHT_TURN_RELAY 5 // Пин реле правого поворотника
#define RIGHT_TURN_SIGNAL 11 // Пин сигнального провода правого поворотника

// Пины прерывания для считывания работы поворотника
#define LEFT_TURN_COUNT 2
#define RIGHT_TURN_COUNT 3


#define BLINK_TIME 3000 // Время работы поворотников
#define TURNS_COUNT 3 // этим дефайном мы выставляем количество морганий поворотника

// Константы для удобства
#define LEFT 1
#define RIGHT 2

unsigned long turn_timer; // переменная таймера для выключения поворотников
int turn_switch = 0; // переменная для определения поворотников
boolean turn_mode = false; // переменная для определения работы поворотников

// Переключение режимов поворотников
boolean turn_mode_switch = true;

// переменные для считывания количества раз моргани поворотников
int left_count = 0;
int right_count = 0;

void setup()
{
  Serial.begin(9600);

#if (DEBUG == 1)
  Serial.println(F("[DEBUG]: Arduino started"));
#endif

  // устанавливаем режим работы входов/выходов
  pinMode(LEFT_TURN_RELAY, OUTPUT);
  pinMode(LEFT_TURN_SIGNAL, INPUT);

  pinMode(RIGHT_TURN_RELAY, OUTPUT);
  pinMode(RIGHT_TURN_SIGNAL, INPUT);
}

void loop()
{
  TurnStart(); // Запускаем считывание сигналов поворотников
  if (digitalRead(LEFT_TURN_SIGNAL) == HIGH) // Если левый поворотник был затронут то выполняем следующие действия
  {
#if (DEBUG == 1)
    Serial.println(F("[DEBUG]: Left turn signal active"));
#endif
    turn_mode = true; // включаем режим работы поворотника
    turn_switch = LEFT; // указываем что работает левый поворотник
  }
  if (digitalRead(RIGHT_TURN_SIGNAL) == HIGH) // Если правый поворотник был затронут то выполняем следующие действия
  {
#if (DEBUG == 1)
    Serial.println(F("[DEBUG]: Right turn signal active"));
#endif
    turn_mode = true; // выключаем режим работы поворотника
    turn_switch = RIGHT; // указываем что работает правый поворотник
  }
}
void TurnStart()
{
  if (turn_mode == true) // проверяем работают ли поворотники
  {
    switch (turn_switch)
    {
      case LEFT:
        {
          digitalWrite(LEFT_TURN_RELAY, HIGH); // включаем реле левого поворотника
          if (turn_mode_switch == false) StartTimeout(LEFT); // если включен режим по таймингу то запускаем обратный отсчет
          else attachInterrupt(0, StartTurnCount, RISING); // если включен режим по подсчету сигналов с поворотников, то запускаем прерывания
          break;
        }
      case RIGHT:
        {
          digitalWrite(RIGHT_TURN_RELAY, HIGH); // включаем реле правого поворотника
          if (turn_mode_switch == false) StartTimeout(RIGHT); // если включен режим по таймингу то запускаем обратный отсчет
          else attachInterrupt(1, StartTurnCount, RISING); // если включен режим по подсчету сигналов с поворотников, то запускаем прерывания
          break;
        }
    }
  }
}
void StartTurnCount()
{
  if (turn_mode == true) // снова проверяем работу поворотников
  {
    switch (turn_switch)
    {
      case LEFT:
        {
          left_count++;
          if (left_count >= TURNS_COUNT)
          {
            digitalWrite(LEFT_TURN_RELAY, LOW);
            left_count = 0;
            turn_mode = false;
#if (DEBUG == 1)
            Serial.println(F("[DEBUG]: Left turn mode deactive"));
#endif
          }
          break;
        }
      case RIGHT:
        {
          right_count++;
          if (right_count >= TURNS_COUNT)
          {
            digitalWrite(RIGHT_TURN_RELAY, LOW);
            right_count = 0;
            turn_mode = false;
#if (DEBUG == 1)
            Serial.println(F("[DEBUG]: Right turn mode deactive"));
#endif
          }
          break;
        }
    }
  }
}
void StartTimeout(int turns)
{
  if (turn_mode == true)
  {
    if (millis() - turn_timer > BLINK_TIME)
    {
      turn_timer = millis();
      switch (turns)
      {
        case LEFT:
          {
#if (DEBUG == 1)
            Serial.println(F("[DEBUG]: Left turn mode deactive"));
#endif
            digitalWrite(LEFT_TURN_RELAY, LOW);
            turn_mode = false;
            break;
          }
        case RIGHT:
          {
#if (DEBUG == 1)
            Serial.println(F("[DEBUG]: Right turn mode deactive"));
#endif
            digitalWrite(RIGHT_TURN_RELAY, LOW);
            turn_mode = false;
            break;
          }
      }
    }
  }
}
