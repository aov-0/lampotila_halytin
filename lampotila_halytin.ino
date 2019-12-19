#include <LiquidCrystal.h>            //Hakee LCD näytön tarvitseman kirjaston käyttöön
LiquidCrystal lcd(9, 8, 7, 6, 5, 4);  //LCD näytön käyttämät pinnit

#define ANALOGPIN A0    //Määrittää A0 pinnin nimeksi ANALOGPIN
#define PIEZO 2         //Määrittää 2 pinnin PIEZO nimiseksi
#define LEDVIHREA 11      //Määrittää 11 pinniin LEDVIHREA
#define LEDKELTAINEN 12   //Määrittää 12 pinniin LEDKELTAINEN
#define LEDPUNAINEN 13    //Määrittää 13 pinniin LEDPUNAINEN

int analogVal;    //Määritetään globaalimuuttuja

void setup(){
  pinMode(PIEZO, OUTPUT);         //Piipperi asetaan OUTPUT:iksi, eli sen kautta johdetaan virtaa ulos
  pinMode(LEDVIHREA, OUTPUT);     //Asetetaan led on ulostuloksi
  pinMode(LEDKELTAINEN, OUTPUT);  //-..-
  pinMode(LEDPUNAINEN, OUTPUT);   //-..-
  lcd.begin(16, 2);               //Otetaan lcd näyttö käyttöön, ts. käynnistetään se
  lcd.print("Lampotila:");         //Kirjoitetaan teksti LCD-näytön yläriville
  Serial.begin(9600);             //Käynnistää sarjaportin
}

void matala(){          //Piipperin oma funktio matalimmalle lämpötilalle
    tone(PIEZO, 400);   //Tone komento soittaa äänen PIEZO:lle valitulla pinnillä, 400hz:llä)
    delay(30);          //Ääni vaatii pienen viiveen, muuten ääni ei ehdi kuulua
    noTone(PIEZO);      //noTone komento haetaan siksi että saadaan piippaava ääni, eli ääni katkeaa välillä
    delay(30);          //Tällekin annetaan pieni viive siksi ettei hiljainen jakso mene ohi liian nopeasti
}

void keskimatala(){   //Vastaava kuin ylemmät
    tone(PIEZO, 800);
    delay(20);
    noTone(PIEZO);
    delay(20);
}

void korkea(){        //Vastaava kuin ylemmät
    tone(PIEZO, 1200);
    delay(15);
    noTone(PIEZO);
    delay(15); 
}

float mittaus(){
  analogVal = analogRead(ANALOGPIN);        //Lukee A0 pinnin analogisen lukeman ja määrittää sen analogVal nimelle
  return (-9.00/70.00)*analogVal+91;  //float mahdollistaa desimaalilukujen käytön. Laskukaavassa on käytetty suoran yhtälöä
  
}

void naytto(float x){
  Serial.println(x);                    //Tulostaa sarjaporttiin ylemmän laskukaavan tuloksen. 
  delay(100);                              //viive jottei lukemia tule jatkuvalla syötöllä
  lcd.setCursor(0,1);                     //Asettaa tekstin alkamaan ensimmäisestä ruudusta vaakatasossa ja toiselta riviltä pystysuunnassa
  lcd.print(x);                        //Tulostaa LCD-näytölle ylempänä olevan laskukaavan tuloksen
  lcd.print(" astetta");                //Tulostetun lukeman perään lisätty teksti selkeyttämään lukemista
}

void ohjaus(float aste){
    if(aste > 27){                      //Jos lukema on enemmän kuin 27,
    digitalWrite(LEDVIHREA, LOW);     //sammutetaan ledit joiden ei haluta palavan
    digitalWrite(LEDKELTAINEN, LOW);  //-..-
    korkea();                         //ajetaan äänimerkin funktio
    digitalWrite(LEDPUNAINEN, LOW);   //Jottei valo palaisi jo ennestään, asetaan se vielä pois päältä tässä vaiheessa
    digitalWrite(LEDPUNAINEN, HIGH);  //Tässä vaiheessa sytytetään LED palamaan
  }else if(aste > 26){                //Jos taas lukema on yli 26
    digitalWrite(LEDVIHREA, LOW);     //Varmistetaan ettei ledit jää turhaan palamaan
    digitalWrite(LEDPUNAINEN, LOW);   //-..-
    keskimatala();                    //ajetaan äänimerkin funktio
    digitalWrite(LEDKELTAINEN, LOW);  //"Pidetään" haluttu ledi vielä pimeänä tässä vaiheessa
    digitalWrite(LEDKELTAINEN, HIGH); //Sytytetään haluttu ledi
  }else if(aste > 25){                //
    matala();
    digitalWrite(LEDKELTAINEN, LOW);
    digitalWrite(LEDPUNAINEN, LOW);
    digitalWrite(LEDVIHREA, HIGH);
  }else{
      digitalWrite(LEDVIHREA, LOW);   //Jottei mikään ledi jää palamaan, varmistetaan se sammuttamalla vihreä vielä tässä
  }
}
 
void loop() {
  float a = mittaus();
  naytto(a);
  ohjaus(a);

}
