/* Robot_master
 *  Elijah Sexton
 *  Robert Sexton

*/

//#include <Servo.h>
#include <Adafruit_TiCoServo.h>
#include <Adafruit_NeoPixel.h>
#include <WaveHC.h>
#include <WaveUtil.h>
#define playcomplete(x) ROM_playcomplete(PSTR(x))     
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6  // output pin to neopixels


SdReader card;    // This object holds the information for the card
FatVolume vol;    // This holds the information for the partition on the card
FatReader root;   // This holds the information for the volumes root directory
FatReader file;   // This object represent the WAV file 
WaveHC wave;      // This is the only wave (audio) object, since we will only play on

Adafruit_NeoPixel strip = Adafruit_NeoPixel(37, PIN, NEO_GRB + NEO_KHZ800);  //setup neopixels

//Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

Adafruit_TiCoServo myservo; // create servo object to control a servo

int pos = 90;    // variable to store the servo position
int sweep = 0;  //variable to store number of sweeps


void setup() {
  int pumpkinstate = 0;
  pinMode(2, OUTPUT); 
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
    Serial.begin(9600);      // open the serial port at 9600 bps:    

  strip.begin();  // neopixels
  strip.show(); // Initialize all pixels to 'off'
  randomSeed(analogRead(0));


  if (!card.init()) {
    Serial.println(F("Card init. failed!")); return;
  }
  // enable optimize read - some cards may timeout. Disable if you're having problems
  card.partialBlockRead(true);
  
  // Now we will look for a FAT partition!
  uint8_t part;
  for (part = 0; part < 5; part++) {   // we have up to 5 slots to look in
    if (vol.init(card, part)) 
      break;                           // we found one, lets bail
  }
  if (part == 5) {                     // if we ended up not finding one  :(
    Serial.println(F("No valid FAT partition!"));  // Something went wrong, lets print out why
  }
  
  // Lets tell the user about what we found
  putstring("Using partition ");
  Serial.print(part, DEC);
  Serial.print(F(", type is FAT"));
  Serial.println(vol.fatType(), DEC);     // FAT16 or FAT32?
  
  // Try to open the root directory
  if (!root.openRoot(vol)) {
    Serial.println(F("Can't open root dir!"));      // Something went wrong,
  }
  
  // Whew! We got past the tough parts.
  Serial.println(F("Files found (* = fragmented):"));

  // Print out all of the files in all the directories.
  root.ls(LS_R | LS_FLAG_FRAGMENTED);
}


void loop() {
  int pumpkinstate = 0;
  int distsensor, i;
  for (sweep = 0; sweep < 1; sweep++) { // sweep 2 times then go to neopixels
   sweephalfright();int distsensor, i;
   long time;
   /*
   for (i=0; i<50; i++) {
     pulseServo(servo,0);
   }
   for (i=0; i<50; i++) {
     pulseServo(servo,400);
   }
   return;
   */
   distsensor = 0;
   for (i=0; i<8; i++) {
     distsensor += analogRead(0);
     delay(50);
   }
   distsensor /= 8;

   Serial.print(F("Sensor = ")); Serial.println(distsensor);
   
   sweepleft();
   sweepright();
   sweepleft();
   sweepmiddle();
   strip.setPixelColor(35, 0, 0, 0);
   strip.setPixelColor(36, 0, 0, 0);
   if (distsensor < 50){
    playcomplete("robot1.wav");
    strip.setPixelColor(35, 255, 0, 0);
    strip.setPixelColor(36, 255, 0, 0);
    colorWipe(strip.Color(255, 0, 0, 255), 100); // Green
   }
   else if (distsensor < 200){
    playcomplete("robot2.wav");
    strip.setPixelColor(35, 0, 155, 0);
    strip.setPixelColor(36, 0, 155, 0);
    colorWipe(strip.Color(0, 155, 0, 0), 100); // Green
   }
   else if (distsensor > 300){
    playcomplete("robot3.wav");
    strip.setPixelColor(35, 0, 255, 0);
    strip.setPixelColor(36, 0, 255, 0);
    colorWipe(strip.Color(0, 155, 0, 0), 100); // Green
   }
   
   
   
   
  }
  //myservo.write(90);

  colorExit();
  //colorWipe(strip.Color(0, 155, 0, 0), 100); // Green
  colorWipeclear(strip.Color(0, 0, 0, 0), 100); // Off
  //colorWipereverse(strip.Color(0, 128, 0, 0), 100); // Green
  //colorWipereverseclear(strip.Color(0, 0, 0, 0), 100); // Off
  
   



}


void playfile(char *name) {

   if (!file.open(root, name)) {
      Serial.println(F(" Couldn't open file")); return;
   }
   if (!wave.create(file)) {
     Serial.println(F(" Not a valid WAV")); return;
   }
   // ok time to play!
   wave.play();
}


 void sweephalfright() {
  for (pos = 90; pos <= 120; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(30);                       // waits 15ms for the servo to reach the position
  }
 }

 void sweepleft() {
  for (pos = 120; pos >= 60; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(30);                       // waits 15ms for the servo to reach the position
  }
 }

  void sweepright() {
  for (pos = 60; pos <= 120; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(30);                       // waits 15ms for the servo to reach the position
  }
 }

 void sweepmiddle() {
  for (pos = 60; pos <= 90; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(30);                       // waits 15ms for the servo to reach the position
  }
 }
 
 // Neopixel subroutines

 void colorExit() {
 strip.setPixelColor(32, 0, 155, 0);
 strip.setPixelColor(33, 0, 155, 0);
 strip.setPixelColor(34, 0, 155, 0);
 //strip.setPixelColor(35, 0, 255, 0);
 //strip.setPixelColor(36, 0, 255, 0);
 
 strip.show();
 }
 
 // Fill the dots one after the other with a color starting with 0
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels()-5; i++) {  
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
    }
  }

    // Turn the dots off starting at the max and going to 0
void colorWipeclear(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels()-5; i++) {  
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
    }
  }

  // Fill the dots one after the other with a color starting at the max
void colorWipereverse(uint32_t c, uint8_t wait) {
  for(uint16_t i=32; i<strip.numPixels()-5; i--) { 
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
    }
  }

  // Turn the dots off starting at the 0 and going to max
void colorWipereverseclear(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels()-5; i++) {  
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
    }
  }

void ROM_playcomplete(const char *romname) {
  char name[13], i;
  uint8_t volume;
  int v2;
  
  for (i=0; i<13; i++) {
    name[i] = pgm_read_byte(&romname[i]);
  }
  name[12] = 0;
  Serial.println(name);
  playfile(name);
  while (wave.isplaying) {
   volume = 0;
   for (i=0; i<8; i++) {
     v2 = analogRead(1) - 512;
     if (v2 < 0) 
        v2 *= -1;
     if (v2 > volume)
       volume = v2;
     delay(5);
   }
   if (volume > 200) {
     
   } else {
     
   }
   if (volume > 150) {
     
   } else {
     
   } 
   if (volume > 100) {
     
   } else {
     
   } 
   //Serial.print(F("vol = ")); Serial.println(volume, DEC);
  }
  file.close();
}  


 
  

