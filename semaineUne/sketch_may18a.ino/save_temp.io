// Initialisation des constantes :
const int led = LED_BUILTIN; // Numero de la broche LED verte
const int potar = 0; // Numero de la broche LED verte

// initialisation des indicateurs d'étapes
boolean stepOne, stepTwo = false;

int pos, val1, val2 = 0;

void setup() {
  // parametrage des broches de sortie.
  digitalWrite(led, HIGH);
  
  // activation de l'éntrée pour saisie clavier de l'angle et simuler une rotation du servo moteur
  Serial.begin(9600);
  
  // recuperartion de la position pour initialisation
  pos = map(analogRead(0), 37, 790, 0, 179);
}

void loop() {
  
  //Recuperation de la valeur actuelle
  val1 = map(analogRead(0), 37, 790, 0, 179);
  delay(500);
  //Recuperation de la nouvelle valeur actuelle
  // pour savoir si on tourne encore le servo moteur
  val2 = map(analogRead(0), 37, 790, 0, 179);


  //test si c'est toujours la même valeur
  // sinon c'est qu'on tourne encore le servo moteur
  // ou on ne la pas encore bougé
  if (val1 == val2 && pos != val1) {
    
    pos = val1;
    
    if (stepOne) { // la première étape est déjà passée
      if(stepTwo) { // la deuxième étape est déjà passée
        if(pos > 60 && pos < 120) {
          // etape 3 ok
          Serial.println("Etape 3 : OK");
          Serial.println("Bravo, le coffre s'ouvre" );
          // faire clignoter la LED 3 fois
          for(int i = 0; i < 3; i++){
            digitalWrite(led, LOW);
            delay(500);
            digitalWrite(led, HIGH);
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
