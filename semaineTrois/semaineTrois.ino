#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h>

Servo myservo;

// configuration du reseau wifi
const char* ssid     = "*********";        // Nom du reseau
const char* password = "************";     // cle 

// valeurs pour le serveur Web
const char* host = "api.tom.tools/";
const String nelsonName = "wilson";
const String ressource = "/nelsons/" + nelsonName;
const int httpPort = 80;

const int servoPin = D1; // Pin de la commande du servomoteur

// initialisation des variables pour les posotions
int pos, val1, val2 = 0;

void setup() {
  Serial.begin(115200);
  
  positionner(0); // initialisation de la position
 
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
    
  Serial.print("Connexion TCP au serveur ");
  Serial.println(host);
  HTTPClient http;
  
  
  if(WiFi.status() != WL_CONNECTED) {
  
    http.begin(host, httpPort, ressource);
  
    
      //Recuperation de la valeur actuelle
    val1 = map(analogRead(A0), 37, 790, 0, 179);HTTPClient http;
    delay(500);
    //Recuperation de la nouvelle valeur actuelle
    // pour savoir si on tourne encore le servo moteur
    val2 = map(analogRead(A0), 37, 790, 0, 179);
    Serial.print(val1);
    Serial.print(" / ");
    Serial.print(val2);
    Serial.print(" / ");
    Serial.println(pos);
  
  
  
  
    //test si c'est toujours la même valeur
    // sinon c'est qu'on tourne encore le servo moteur
    // ou on ne la pas encore bougé
    if (val1 == val2 && pos != val1) {
      
      pos = val1;
      if (pos > 90) {
        Serial.println("Etape 1 : OK");
      }
    }
    
    // Lancement de la requete vers le serveur
    Serial.println("[HTTP] PATCH...");
    
    // payload envoye a la ressource que nous requetons
    String payload = "{\"position\":";
    payload += String(pos);
    payload += "}";
  
    Serial.print("[HTTP] PATCH... payload:");
    Serial.println(payload);
    
    // démarrer la connexion et envoyer les entêtes HTTP
    http.addHeader("Content-type", "application/json");
    int httpCode = http.sendRequest("PATCH", (uint8_t *) payload.c_str(), payload.length());
  
    // httpCode sera négatif si on rencontre une erreur
    if(httpCode > 0) {
      // les entêtes HTTP ont été envoyés et
      Serial.printf("[HTTP] PATCH... code: %d\n", httpCode);
      
      // si le serveur TOM répond par OK
      if(httpCode == HTTP_CODE_OK)
        Serial.println("[HTTP] PATCH... OK!");
    } else {
      Serial.printf("[HTTP] PATCH... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    
    http.end();
  
    Serial.println();
    Serial.println("done");
    delay(10000); // attente de 10s avant de relancer la réupération
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


