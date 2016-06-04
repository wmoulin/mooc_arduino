#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h>

Servo myservo;

// configuration du reseau wifi
const char* ssid     = "**********";        // Nom du reseau
const char* password = "*************";     // cle 

// valeurs pour le serveur Web
const String host = "api.tom.tools";
const String nelsonName = "******";
const int httpPort = 80;
const String ressource = "http://" + host + ":" + httpPort + "/nelsons/" + nelsonName;

const int servoPin = D1; // Pin de la commande du servomoteur

// initialisation des variables pour les posotions
int pos0, pos1, pos2 = 0;
int tolerance = 5; // tolerance de 5 degre pour etre sure d'avoir bouge le servomoteur

void setup() {
  
  Serial.begin(115200);
  
  positionner(0); // initialisation de la position a 0 degre
  pos0 = map(analogRead(A0), 37, 790, 0, 179); // lecture de la valeur pour le qualibrage du 0 degré
 
  // connexion au reseau wifi
  Serial.print("Connexion au WiFi ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);    // On lance la connexion au reseau wifi
  
  while (WiFi.status() != WL_CONNECTED) { // On attend la connexion
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connecté");  
}

void loop() {
    
  HTTPClient http;
  
  if(WiFi.status() == WL_CONNECTED) { // verification de la connexion
  
    //Recuperation de la valeur actuelle
    pos1 = map(analogRead(A0), 37, 790, 0, 179); // qualibrage des valeur min et max de mon servomoteur (37, 790 pour le 3.3V)
    delay(500);
    //Recuperation de la nouvelle valeur actuelle
    // pour savoir si on tourne encore le servo moteur
    pos2 = map(analogRead(A0), 37, 790, 0, 179); // qualibrage des valeur min et max de mon servomoteur (37, 790 pour le 3.3V)
  
    //test si c'est toujours la même valeur
    // sinon c'est qu'on tourne encore le servo moteur
    // ou on ne la pas encore bougé
    if (pos1 == pos2 && pos1 != pos0 && (pos1 - pos0 > tolerance || pos0 - pos1 > tolerance) && pos1 > 90) {
      
      Serial.println("[HTTP] PATCH...");
      String payload = "{\"position\":180}";
          
      // démarrer la connexion et envoyer les entêtes HTTP
      http.begin(ressource);
      
      // démarrer la connexion et envoyer les entêtes HTTP
      http.addHeader("Content-type", "application/json");
      int httpCode = http.sendRequest("PATCH", (uint8_t *) payload.c_str(), payload.length());
    
      // httpCode sera négatif si on rencontre une erreur
      if(httpCode > 0) {
        // les entêtes HTTP ont été envoyés et
        Serial.printf("[HTTP] PATCH... code: %d\n", httpCode);
        
        // si le serveur TOM répond par OK
        if(httpCode == HTTP_CODE_OK) {
          Serial.println("[HTTP] PATCH... OK!");
        }
      } else {
        Serial.printf("[HTTP] PATCH... failed, error: %d - %s\n", httpCode, http.errorToString(httpCode).c_str());
      }
      
      http.end();
      positionner(0); // reinitialisation de la position
      pos0 = map(analogRead(A0), 37, 790, 0, 179); // lecture de la valeur pour le qualibrage du 0 degré
   
      Serial.println();
      Serial.println("done");
      delay(3000); // attente de 3s avant de relancer le traitement
    }

  }
  
}

/*
 * Fonction gérant la position du moteur
 */
void positionner(int position) {
  myservo.attach(servoPin);
  myservo.write(position);
  delay(500); // delai pour laisser au moteur le temps de bouger avant le detach
  myservo.detach(); // pour pouvoir le bouger a la main et verifier le bon fonctionnement
}


