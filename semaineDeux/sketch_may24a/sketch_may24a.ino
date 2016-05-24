#include <ESP8266WiFi.h>
#include <Servo.h>

// configuration du reseau wifi
const char* ssid     = "*********";        // Nom du reseau
const char* password = "************";     // cle 

// configuration de la requete
const char* host = "api.openweathermap.org";
const int httpPort = 80;
const char* apikey   = "1a702a15a2f46e405e61804cf67c0d30";
const char* town     = "Nantes,fr";

const String keywordDeg = String("\"deg\":");  //chaîne que l'on recherche dans le JSON

const int servoPin = D1; // Pin de la commande du servomoteur
Servo windServo;  // servo moteur de la girouette

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
  
  // Creation d'une connexion TCP vers le serveur
  WiFiClient client;
  if (!client.connect(host, httpPort)) {
    Serial.println("erreur de connexion");
    return;
  }
  
  // Url de la ressource que nous requetons
  String url = String("/data/2.5/weather?q=") + town + "&appid=" + apikey;
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // Lancement de la requete vers le serveur
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  // timeout de 5s sinon on relance la connexion
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println("Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Taitement de la reponse recue
  while(client.available()){
    String line = client.readStringUntil('\r');
    //Serial.print(line);

    // recuperation de la position de ce que nous recherchons dans la reponse
    int posWind = line.indexOf(keywordDeg);

    if (posWind > 0) { // mot clé trouvé
      // indexOf donne la position du début du mot clé, en ajoutant sa longueur
      // on se place à la fin.
      posWind += keywordDeg.length();       
      int degre = atoi(&line[posWind]); // la partie entière de la valeur nous suffit sinon utiliser atof
      Serial.print("vent : ");
      Serial.println(degre);
      positionner(degre<179?degre:degre-179);
    }

  }
  
  Serial.println();
  Serial.println("done");
  delay(10000); // attente de 10s avant de relancer la réupération
  
}

/*
 * Fonction gérant la position du moteur
 */
void positionner(int position) {
  windServo.attach(servoPin);
  windServo.write(position);
  delay(500); // delai pour laisser au moteur le temps de bouger avant le detach
  windServo.detach(); // pour pouvoir le bouger a la main et verifier le bon fonctionnement
}
