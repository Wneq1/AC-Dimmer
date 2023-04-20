#include  <TimerOne.h>                    //biblioteka do obslugi regulacji fazowej
  
#include <Wire.h>                         //inicjalizacja wyświetlacza
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);
#if (SSD1306_LCDHEIGHT != 64)
//#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif/

int LED =12;


int przycisk_dodaj = 9;                   //przycisk zwiekszajacy moc

int przycisk_odejmij = 7;                 //przycisk zmniejszajacy moc

int przycisk_zapisz =8;                   //przycisk zapisu
int stan_przycisku_zapisz=HIGH;
int ostatni_stan_przycisku_zapisz=HIGH;
int zapis=0;

int Triak = 10;                          //pin do sterowania triakiem

int inc=1;                                // dotyczy zmiennej testowej

volatile int i=0;                         //odlicza czas od przejscia przez zero CHYBA?!?
volatile boolean przejscie_przez_zero=0;            //zmienna dotyczaca przejscia przez zero                
int value = 128;                            //zmienna sluzaca do zmiany wartosci mocy 128- Wylaczone (0%) 0-Wlaczone (100%)                             
int krok_czestotliw = 75;                        //Dla 60 Hz powinno byc zmienione na 65 określa czas czas przed załączniem
 
/* Jest obliczany bazujac na częstotliwości napiecia zasilania i ilosci jasnosci jakie potrzeba
 *  
 *  Program bazuje na dwoch przejsciach przez zero podczas jednego cyklu to znaczy dla 120Hz i 60 hz lub dla 100Hz i 50Hz
 *  
 *  Zeby okreslic krok czestotliwoci nalezy podzielic dlugosc pelnej polowki sygnalu w mikrosekundach przez ilosc jasnosci na krok
 *  
 * (120Hz =  8 333uS)-> 128 kroków daje 65uS
 * (100Hz = 10 000uS)-> 128 kroków daje 75uS
*/

void setup() { 
    
  pinMode(Triak, OUTPUT);                //wybieramay triaka jako wyjscie 

  pinMode(przycisk_dodaj, INPUT_PULLUP);  //podlaczenie przyciskow przez rezystory podciagajace 
  pinMode(przycisk_odejmij, INPUT_PULLUP); 
  pinMode(przycisk_zapisz,INPUT_PULLUP);   


                       
  attachInterrupt(0, wykrycie_przejscia_przez_zero, RISING);  //funkcja odpowiada za detekcje przejscia przez zero podlaczonego do pinu D2->(0)   
  Timer1.initialize(krok_czestotliw);                    //inicjalizacja timera dla czestotliwosci ktorej potrzebujemy 
  Timer1.attachInterrupt(sprawdzenie_value, krok_czestotliw);    //funkcja sprawdza czy triak zostanie zalaczony w odpowiedniej chwili i dziala w milisekundach  
   
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  //inicjalizacja wyswietlacza przez I2C i podaniu jej adresu 0x3D o rozdzielczosci 128x64 
  display.display(); 
  display.clearDisplay();     
                                  
}

void wykrycie_przejscia_przez_zero() {   //funkcja sterujaca triakiem gdy zestonie wykryte przejscie przez 0  
  przejscie_przez_zero = true;               
  i=0;
  digitalWrite(Triak, LOW);      
}                                 


void sprawdzenie_value() {            //funkcja odpowiadajaca za sprawdzenie przejscia przez zero         
  if(przejscie_przez_zero == true) {              
    if(i>=value) {                     
      digitalWrite(Triak, HIGH);       
      i=0;                         
      przejscie_przez_zero = false; 
    } 
    else {
      i++;                    
    }                                
  }                                  
}                                   

int Przycisk_Dodaj (int przycisk_dodaj, int przycisk_zapisz){ //przycisk zwiekszający zmienna value odpowiadajaca za  moc
  if(zapis==1){
  if (digitalRead(przycisk_dodaj) == LOW) { 
    if (value < 128) { 
      value = value + 1;
      delay(10);
        }
      }
  return(value);
   }
 }

int Przycisk_Odejmij(int przycisk_odejmij,int przycisk_zapisz){ //przycisk zmniejszajacy zmienna value odpowiadajaca za  moc
 if(zapis==1){
  if (digitalRead(przycisk_odejmij) == LOW) { 
    if (value > 0) { 
      value=value - 1; 
      delay(10);
        }
      }
  return(value);
   }
 }
 
int Przycisk_zapisz(int przycisk_zapisz){               //przycisk po ktorego wcisnieciu jest mozliwa dopiero zmiana mocy
  stan_przycisku_zapisz=digitalRead(przycisk_zapisz);
  if(stan_przycisku_zapisz==LOW && ostatni_stan_przycisku_zapisz==HIGH){
  zapis = 1 -zapis;
     }
  ostatni_stan_przycisku_zapisz = stan_przycisku_zapisz;
   delay(10);
  return (zapis);
 } 

void test(){                                                   //sekwencyjne zapalanie i gaszenie obciązenia
 value+=inc;
  if((value>=128) || (value<=0))
    inc*=-1;
  delay(20);
}

void loop() { 
  
 Przycisk_zapisz(przycisk_zapisz);                            //inicjalizacja funkcji odpowiadajacych za sterowanie przyciskami
 Przycisk_Dodaj ( przycisk_dodaj,przycisk_zapisz);
 Przycisk_Odejmij( przycisk_odejmij,przycisk_zapisz);
 
  display.clearDisplay();                                     //inicjalizacja wyswietlacza
  display.setTextSize(4);
  display.setTextColor(WHITE);
  display.setCursor(24,0);
  display.print(map(value, 128, 0, 0, 100)); 
  display.print("%");
  display.display();
  if (zapis==1){
    digitalWrite(LED, HIGH);
  }
  else
  digitalWrite(LED, LOW);
     
}
