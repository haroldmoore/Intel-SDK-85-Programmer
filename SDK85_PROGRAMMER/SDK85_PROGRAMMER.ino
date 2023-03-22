/*
 * ORIGINAL CODE BY BEN EATER
 * 
 * MODIFIED BY HAROLD MOORE
 * FOR USE WITH THE 'INTEL SDK-85' DEVELOPMENT SYSTEM
 * WITH '0x' STRING UTILITY ADDED.
 * 
 * 0x converter program for hex strings
 * from eeproms, assemblers, etc. To enable
 * C program strings into the flash programmer
 * module programming the three 28C16 eeproms that
 * ORG at :
 * 
 * CS2 1000-17FF
 * CS6 3000-37FF
 * CS7 3800-3FFF
 * 
 * cut and paste hex object machine code text strings 
 * into the Nano eeprom programmer program, cut and paste
 * the conveted '0x' strings, then re-program the Nano with
 * the new SDK-85 programs using the 'Move' M command to put
 * programs into RAM. A much quicker method than using the
 * very slow 110 baud 'I' command thru the serial port!
 */




#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_EN 13

//------------- EXAMPLE STRING TO HAVE '0x' AND ',' PARSED IN:
char eeprom_string[] = "AABBCCDDEEFFAABBCCDDEEAABBCCDDEE";
byte eeprom_bytes[16];

char ch;
int num = 49; 
char dataz[100];

void eeprom_0x(){
 
for(char i = 0; i < 16; i++)
{
  byte eeprom;
  char a = eeprom_string[2*i];
  char b = eeprom_string[2*i + 1];
  eeprom = Check_If_Hex(a)<<4 | Check_If_Hex(b);
  eeprom_bytes[i] = eeprom;
  Serial.print("0x");
  Serial.print(eeprom_bytes[i], HEX);
  Serial.print(","); 

}

}

/*
 * Output the address bits and outputEnable signal using shift registers.
 */
void setAddress(int address, bool outputEnable) {
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (address >> 8) | (outputEnable ? 0x00 : 0x80));
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);

  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}


/*
 * Read a byte from the EEPROM at the specified address.
 */
byte readEEPROM(int address) {
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    pinMode(pin, INPUT);
  }
  setAddress(address, /*outputEnable*/ true);

  byte data = 0;
  for (int pin = EEPROM_D7; pin >= EEPROM_D0; pin -= 1) {
    data = (data << 1) + digitalRead(pin);
  }
  return data;
}


/*
 * Write a byte to the EEPROM at the specified address.
 */
void writeEEPROM(int address, byte data) {
  setAddress(address, /*outputEnable*/ false);
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    pinMode(pin, OUTPUT);
  }

  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    digitalWrite(pin, data & 1);
    data = data >> 1;
  }
  digitalWrite(WRITE_EN, LOW);
  delayMicroseconds(1);
  digitalWrite(WRITE_EN, HIGH);
  delay(10);
}


/*
 * Read the contents of the EEPROM and print them to the serial monitor.
 */
void printContents() {
  for (int base = 0; base <= 512; base += 16) {
    byte data[16];
    for (int offset = 0; offset <= 15; offset += 1) {
      data[offset] = readEEPROM(base + offset);
    }

    char buf[80];
    sprintf(buf, "%03x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

    Serial.println(buf);
  }
}

//        -----------------------------test pattern--------------------------------
//------------------------------------------------------------------------------------------------------------------
  byte data[] = { 0x85, 0x48, 0x20, 0x20, 0x48, 0x20, 0x20, 0x20, 0x41, 0x20, 0x20, 0x20, 0x52, 0x52, 0x52, 0x20 };
 byte data1[] = { 0x20, 0x20, 0x4F, 0x4F, 0x20, 0x20, 0x4C, 0x20, 0x20, 0x20, 0x20, 0x44, 0x44, 0x20, 0x20, 0x0D };
 byte data2[] = { 0x85, 0x48, 0x20, 0x20, 0x48, 0x20, 0x20, 0x41, 0x20, 0x41, 0x20, 0x20, 0x52, 0x20, 0x20, 0x52 };
 byte data3[] = { 0x20, 0x4F, 0x20, 0x20, 0x4F, 0x20, 0x4C, 0x20, 0x20, 0x20, 0x20, 0x44, 0x20, 0x44, 0x20, 0x0D };
 byte data4[] = { 0x85, 0x48, 0x20, 0x20, 0x48, 0x20, 0x41, 0x20, 0x20, 0x20, 0x41, 0x20, 0x52, 0x20, 0x20, 0x52 };

 byte data5[] = { 0x20, 0x4F, 0x20, 0x20, 0x4F, 0x20, 0x4C, 0x20, 0x20, 0x20, 0x20, 0x44, 0x20, 0x20, 0x44, 0x0D };
 byte data6[] = { 0x85, 0x48, 0x48, 0x48, 0x48, 0x20, 0x41, 0x20, 0x20, 0x20, 0x41, 0x20, 0x52, 0x52, 0x52, 0x20 };
 byte data7[] = { 0x20, 0x4F, 0x20, 0x20, 0x4F, 0x20, 0x4C, 0x20, 0x20, 0x20, 0x20, 0x44, 0x20, 0x20, 0x44, 0x0D };
 byte data8[] = { 0x85, 0x48, 0x20, 0x20, 0x48, 0x20, 0x41, 0x41, 0x41, 0x41, 0x41, 0x20, 0x52, 0x52, 0x20, 0x20 };
 byte data9[] = { 0x20, 0x4F, 0x20, 0x20, 0x4F, 0x20, 0x4C, 0x20, 0x20, 0x20, 0x20, 0x44, 0x20, 0x20, 0x44, 0x0D };

byte data10[] = { 0x85, 0x48, 0x20, 0x20, 0x48, 0x20, 0x41, 0x20, 0x20, 0x20, 0x41, 0x20, 0x52, 0x20, 0x52, 0x20 };
byte data11[] = { 0x20, 0x4F, 0x20, 0x20, 0x4F, 0x20, 0x4C, 0x20, 0x20, 0x20, 0x20, 0x44, 0x20, 0x20, 0x44, 0x0D };
byte data12[] = { 0x85, 0x48, 0x20, 0x20, 0x48, 0x20, 0x41, 0x20, 0x20, 0x20, 0x41, 0x20, 0x52, 0x20, 0x20, 0x52 };
byte data13[] = { 0x20, 0x4F, 0x20, 0x20, 0x4F, 0x20, 0x4C, 0x20, 0x20, 0x20, 0x20, 0x44, 0x20, 0x44, 0x20, 0x0D };
byte data14[] = { 0x85, 0x48, 0x20, 0x20, 0x48, 0x20, 0x41, 0x20, 0x20, 0x20, 0x41, 0x20, 0x52, 0x20, 0x20, 0x52 };

byte data15[] = { 0x20, 0x20, 0x4F, 0x4F, 0x20, 0x20, 0x4C, 0x4C, 0x4C, 0x4C, 0x20, 0x44, 0x44, 0x20, 0x0D, 0x0D };
//-------------------------------------------------------------------------------------------------------------------
byte data16[] = { 0x0D, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
byte data17[] = { 0x31, 0xC2, 0x20, 0x21, 0x00, 0x28, 0x7E, 0x4F, 0x23, 0xFE, 0x00, 0xCA, 0x10, 0x20, 0x07, 0xDA };
byte data18[] = { 0x34, 0x20, 0x0F, 0xFE, 0x0D, 0xCA, 0x2E, 0x20, 0xCD, 0xFA, 0x07, 0xC3, 0x16, 0x20, 0xCD, 0xEB };
byte data19[] = { 0x05, 0xC3, 0x16, 0x20, 0x0F, 0xE6, 0x7F, 0x57, 0x0E, 0x20, 0xCD, 0xFA, 0x07, 0x15, 0xC2, 0x38 };
byte data20[] = { 0x29, 0xC3, 0x16, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
byte data21[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };



//----------------------------------setup-----------------------------------
void setup() {
  // put your setup code here, to run once:
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  digitalWrite(WRITE_EN, HIGH);
  pinMode(WRITE_EN, OUTPUT);
  Serial.begin(9600);

 // erase_all();
 // program_data();
 // readEE();

 sign_on();
}

void erase_all(){
  // Erase entire EEPROM
  Serial.print("Erasing EEPROM");
  for (int address = 0; address <= 512; address += 1) {
    writeEEPROM(address, 0xff);

    if (address % 64 == 0) {
      Serial.print(".");
    }
  }
  Serial.println(" done"); 
}

void program_data(){
// Program data bytes
  Serial.print("Programming EEPROM");
  for (int address = 0; address < sizeof(data); address += 1) {

    //-------------------------------------------------------------------------

    
    writeEEPROM(address, data[address]);
    writeEEPROM(address + 16, data1[address]);
    writeEEPROM(address + 32, data2[address]);
    writeEEPROM(address + 48, data3[address]);
    writeEEPROM(address + 64, data4[address]);

    writeEEPROM(address + 80, data5[address]);
    writeEEPROM(address + 96, data6[address]);
    writeEEPROM(address + 112, data7[address]);
    writeEEPROM(address + 128, data8[address]);
    writeEEPROM(address + 144, data9[address]);

    writeEEPROM(address + 160, data10[address]);
    writeEEPROM(address + 176, data11[address]);
    writeEEPROM(address + 192, data12[address]);
    writeEEPROM(address + 208, data13[address]);
    writeEEPROM(address + 224, data14[address]);

    writeEEPROM(address + 240, data15[address]);
    writeEEPROM(address + 256, data16[address]);

    
    writeEEPROM(address + 272, data17[address]);
    writeEEPROM(address + 288, data18[address]);
    writeEEPROM(address + 304, data19[address]);
    writeEEPROM(address + 320, data20[address]);
    writeEEPROM(address + 336, data21[address]);

       
    
    if (address % 64 == 0) {
      Serial.print(".");
    }
  }
  Serial.println(" done");  
}

void program_data1(){
// Program data bytes
  Serial.print("Programming EEPROM");
  for (int address = 0; address < sizeof(data1); address += 1) {
   writeEEPROM(address + 102, data7[address]);
    if (address % 64 == 0) {
      Serial.print(".");
    }
  }
  Serial.println(" done");  
}




void readEE(){
  // Read and print out the contents of the EERPROM
  Serial.println("Reading EEPROM");
  printContents();
}

void sign_on(){
   Serial.println("-------------------------"); 
   Serial.println("A --> readEE();"); 
   Serial.println("B --> UNUSED"); 
   Serial.println("C --> erase_all();"); 
   Serial.println("D --> program_data();"); 
   Serial.println("E --> this menu"); 
   Serial.println("F --> '0x' String Utility"); 
}



void loop() {
  
  // CALL THE BASIC PROGRAMMER FUNCTIONS:

 if (Serial.available()) {
        
             while (Serial.available()) {
                   ch = (Serial.read());
                    switch (ch){
                      
                      case 65: //A
                             readEE();
                      break;
                      case 66: //B
                          //   writeEEPROM(0x00, 0xAA);
                      break;
                      case 67: //C
                             erase_all();
                      break;
                      case 68: //D
                             program_data();
                      break;
                      case 69: //E
                            sign_on();
                           //  program_data1();
                      break;
                      case 70: //F
                           //  sign_on();
                           eeprom_0x();
                          
                      break;

                    
                   }
             }
        }
}



char Check_If_Hex(char ch)
{
  char returnType;
  switch(ch)
  {
    case '0':
    returnType = 0;
    break;
    case  '1' :
    returnType = 1;
    break;
    case  '2':
    returnType = 2;
    break;
    case  '3':
    returnType = 3;
    break;
    case  '4' :
    returnType = 4;
    break;
    case  '5':
    returnType = 5;
    break;
    case  '6':
    returnType = 6;
    break;
    case  '7':
    returnType = 7;
    break;
    case  '8':
    returnType = 8;
    break;
    case  '9':
    returnType = 9;
    break;
    case  'A':
    returnType = 10;
    break;
    case  'B':
    returnType = 11;
    break;
    case  'C':
    returnType = 12;
    break;
    case  'D':
    returnType = 13;
    break;
    case  'E':
    returnType = 14;
    break;
    case  'F' :
    returnType = 15;
    break;
    default:
    returnType = 0;
    break;
  }
  return returnType;
}
