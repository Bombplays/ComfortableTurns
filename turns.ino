#define LEFT_TURN_RELAY 4
#define LEFT_TURN_SIGNAL 10
#define RIGHT_TURN_RELAY 5
#define RIGHT_TURN_SIGNAL 11

#define BLINK_TIME 2000

#define LEFT 1
#define RIGHT 2

unsigned long turn_timer;
int turn_switch = 0;
boolean turn_mode = false;

void setup()
{
  Serial.begin(9600);
  pinMode(LEFT_TURN_RELAY, OUTPUT);
  pinMode(LEFT_TURN_SIGNAL, INPUT);

  pinMode(RIGHT_TURN_RELAY, OUTPUT);
  pinMode(RIGHT_TURN_SIGNAL, INPUT);
}

void loop()
{
  TurnStart();
  if (digitalRead(LEFT_TURN_SIGNAL) == HIGH)
  {
    turn_mode = true;
    turn_switch = LEFT;
  }
  if (digitalRead(RIGHT_TURN_SIGNAL) == HIGH)
  {
    turn_mode = true;
    turn_switch = RIGHT;
  }
}
void TurnStart()
{
  if (turn_mode == true)
  {
    if (turn_switch == LEFT)
    {
      digitalWrite(LEFT_TURN_RELAY, HIGH);
      StartTimeout(LEFT);
    }
    if (turn_switch == RIGHT)
    {
      digitalWrite(RIGHT_TURN_RELAY, HIGH);
      StartTimeout(RIGHT);
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
            digitalWrite(LEFT_TURN_RELAY, LOW);
            turn_mode = false;
            break;
          }
        case RIGHT:
          {
            digitalWrite(RIGHT_TURN_RELAY, LOW);
            turn_mode = false;
            break;
          }
      }
    }
  }
}
