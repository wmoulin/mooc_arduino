// Initialisation des constantes :
const int greenLed = LED_BUILTIN; // Numero de la broche LED verte
const int potar = 0; // Numero de la broche LED verte
int pos, val1, val2 = 0;


// initialisation des indicateurs d'étapes
boolean stepOne, stepTwo = false;

void setup() {
  // parametrage des broches de sortie.
  //pinMode(greenLed, OUTPUT);
  
  // activation de l'éntrée pour saisie clavier de l'angle et simuler une rotation du servo moteur
  Serial.begin(9600);
  pos = map(analogRead(0), 37, 790, 0, 179);
}

void loop() {
  
  val1 = map(analogRead(0), 37, 790, 0, 179);
  delay(500);
  val2 = map(analogRead(0), 37, 790, 0, 179);


  // une valeur a été saisie
  if (val1 == val2 && pos != val1) {
    pos = val1;
    Serial.print("valeur : ");
    
    if (stepOne) { // la première étape est déjà passée
      if(stepTwo) { // la deuxième étape est déjà passée
        if(pos > 60 && pos < 120) {
          // etape 3 ok
          Serial.println("Etape 3 : OK");
          Serial.println("Bravo, le coffre s'ouvre" );
          // faire clignoter la LED 3 fois
          for(int i = 0; i < 3; i++){
            digitalWrite(greenLed, HIGH);
            delay(500);
            digitalWrite(greenLed, LOW);
            delay(500);
          }
          stepOne = stepTwo = false; // reinitialisation des indicateurs
        } else {
          stepOne = stepTwo = false; // reinitialisation des indicateurs, une étape est KO
        }
      } else if (pos < 60) {
        Serial.println("Etape 2 : OK");
    stepTwo = true; // etape 2 ok
      } else {
        stepOne = stepTwo = false; // reinitialisation des indicateurs, une étape est KO
      }
    } else if (pos > 120){
      Serial.println("Etape 1 : OK");
      stepOne = true; // etape 1 ok
    }
  }
}
