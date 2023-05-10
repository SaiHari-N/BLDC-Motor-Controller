#define HALL_U 8
#define HALL_V 9
#define HALL_W 10

char incomingByte;

void 
setup() 
{
  Serial.begin(9600);
  DDRB = 0x07;
  Serial.println("please enter the hall sensors state:");
}

void 
loop() 
{
  // reply only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    switch (incomingByte)
    {
      case '1':
        PORTB = 0b011;
      break;
      case '2':
        PORTB = 0b010;
      break;
      case '3':
        PORTB = 0b110;
      break;
      case '4':
        PORTB = 0b100;
      break;
      case '5':
        PORTB = 0b101;
      break;
      case '6':
        PORTB = 0b001;
      break;

      default:
      break;
    }
    Serial.println(incomingByte);
    Serial.println("please enter the hall sensors state:");
  }
}
