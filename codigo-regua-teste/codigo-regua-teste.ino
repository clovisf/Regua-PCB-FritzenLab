// Conexão do termistor
const int pinTermistor = A1;
const int referencia = A2;
 
// Parâmetros do termistor
const double beta = 4000.0;
const double r0 = 10000.0;
const double t0 = 273.0 + 25.0;
const double rx = r0 * exp(-beta/t0);
 
// Parâmetros do circuito
double vcc = 3.2;
const double R = 10000.0;
 
// Numero de amostras na leitura
const int nAmostras = 5;

int bitsresultados[6];
int passagem= 0;

// Variables used on this code
unsigned long time1;
unsigned long previousTime;
boolean enterFunction = true;
unsigned long time2;
unsigned long previousTime2;
boolean enterFunction2 = true;
//-----------------------

bool blinkingtens= false;
bool enteredtens= false;
int doubletens= 0;
bool blinkingunits = false;
bool enteredunit = false;
int doubleunit = 0;
int unit = 0;
int tens = 0;
bool waittime = true;
bool startedwait = false;
long elapsedtime;
int temperature;

int digitoaentrar = 1;


void setup() {
  
  //Serial.begin(9600);
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
}

void loop() {

  time1 = micros();
  time2 = micros();
  if (enterFunction == true) {
    previousTime = time1;
    passagem++;
    // Start your code below
    //-----------------------

    if(blinkingtens == true && blinkingunits == false){
      if(enteredtens == false){
        enteredtens = true;
        doubletens = 2 * tens;
        if(doubletens == 0){
          doubletens= 1;
        }
      } 
      doubletens --;
      if(doubletens != 0){
        digitalWrite(0, !digitalRead(0));
      }else{
        blinkingunits = true;
        blinkingtens = false;
      }

    }else if(blinkingunits == true && blinkingtens == false){
      blinkingtens= false;
      if(enteredunit == false){
        enteredunit = true;
        doubleunit = 2 * unit;
        if(doubleunit == 0){
          doubleunit= 1;
        }
      }
      doubleunit --;
      if(doubleunit != 0){
        digitalWrite(1, !digitalRead(1));
      }else{
        waittime = true;
        blinkingunits = false;
        startedwait = true;
      }

    }else if(waittime == true){
      if(startedwait == true){
        digitalWrite(1, LOW);
        digitalWrite(0, LOW);
        startedwait = false;
        elapsedtime = millis();
      }
      if(millis() - elapsedtime > 998){
        waittime = false;
      }

    }else{
      blinkingtens= true;
      enteredtens = false;
      enteredunit = false;
    }

    
    
      
    

    //-----------------------
    // End of your code
  }

  if (enterFunction2 == true && waittime == true) { //Enter this function every xx milisseconds and IF LEDs are not blinking
    previousTime2 = time2;

    // Le o sensor algumas vezes
    int soma = 0;
    for (int i = 0; i < nAmostras; i++) {
      soma += analogRead(pinTermistor);
      delay (100);
    }

    vcc= 2 * analogRead(referencia) * 5 / 1023;
    // Determina a resistência do termistor
    double v = (vcc*soma)/(nAmostras*1024.0);
    double rt = (vcc*R)/v - R;
 
    // Calcula a temperature
    double t = beta / log(rt/rx);
    t= t - 273;
    temperature = int(t);
    //Serial.println (t-273.0);

    if(temperature < 10){
      tens= 0;
      unit= temperature;
    }else if(temperature >= 10 && temperature < 20){
       tens= 1;
       unit= temperature - 10;
    }else if(temperature >= 20 && temperature < 30){
      tens= 2;
      unit= temperature - 20;
    }else if(temperature >= 30 && temperature < 40){
      tens= 3;
      unit= temperature - 30;
    }else if(temperature >= 40 && temperature < 50){
      tens= 4;
      unit= temperature - 40;
    }else{
      tens= 0;
      unit= 0;
    }
  }


  // The DELAY time is adjusted in the constant below >>
  if (time1 - previousTime < 399990) { // 1 million microsencods= 1 second delay
    /* I have actually used 0.999990 seconds, in a trial to compensate the time that
       this IF function takes to be executed. this is really a point that
       need improvement in my code */
    enterFunction = false;
  }
  else {
    enterFunction = true;
  }
  if (time2 - previousTime2 < 99990) { // 1 million microsencods= 1 second delay
    /* I have actually used 0.999990 seconds, in a trial to compensate the time that
       this IF function takes to be executed. this is really a point that
       need improvement in my code */
    enterFunction2 = false;
  }
  else {
    enterFunction2 = true;
  }

}
