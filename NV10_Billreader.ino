#define NoteAcceptor Serial3

/* 
 *  control a NV10 Bank Note Validating System with a Arduino Mega 2560
 *  NV10 PIN 1 (RS232) Tx <-> Arduino Serial3 Rx 15
 *  NV10 PIN 5 (RS232) Rx <-> Arduino Serial3 Tx 14
 *  Make sure both grounds (NV 10 PIN 16 <-> Arduino GND) are connected
 *  Documentation https://www.coinoperatorshop.com/media/products/NV10%20Manual%20Deutsch.pdf
 *  page 16 shows the messages which can be send and received
 */

byte noteInEscrow = 0;

int credit = 0;

byte channelValues[] = {5, 10, 20};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  NoteAcceptor.begin(300, SERIAL_8N2);

  pinMode(15, INPUT_PULLUP);

  NoteAcceptor.write(184);
  NoteAcceptor.write(170);
  NoteAcceptor.write(191);

}

void loop() {
  // put your main code here, to run repeatedly:
  if (NoteAcceptor.available()){
    byte byteIn = NoteAcceptor.read();
    Serial.print("**IN-");
    Serial.print(byteIn);
    Serial.println("**");

    if (byteIn >= 1 && byteIn <= 3){
      if (noteInEscrow){
        Serial.println("Escrow accepted");
        credit = credit + channelValues[byteIn - 1];

        Serial.print("Credit: €");
        Serial.println(credit);
        noteInEscrow = 0;
      }else{
        Serial.print("€");
        Serial.print(channelValues[byteIn - 1]);
        Serial.println(" in escrow");
        Serial.println("Accept 'Y' or 'N'");
        noteInEscrow = byteIn;
      }


    }
    if (byteIn == 70){
       Serial.println("Abort from escrow");
       noteInEscrow = 0;
    }
  }

  if (Serial.available()){
    byte byteIn = Serial.read();
    if (byteIn == 'Y'){
      NoteAcceptor.write(172);
      Serial.println("Accept");
    }
    if (byteIn == 'N'){
      NoteAcceptor.write(173);
      Serial.println("Decline");
    }
    if (byteIn == 'R'){
      credit = 0;
      Serial.println("Credit reset.");
    }
    if (byteIn == 'E'){
      NoteAcceptor.write(184);
      Serial.println("Acceptor enabled");
    }
    if (byteIn == 'D'){
      NoteAcceptor.write(185);
      Serial.println("Acceptor Disabled");
    }
    if (byteIn == 'M'){
      NoteAcceptor.write(170);
      Serial.println("Enable serial escrow mode");
    }
    if (byteIn == 'S'){
      NoteAcceptor.write(182);
      Serial.println("Status");
    }
  }
}
