/* LISTE DES SHIELDS UTILISES */
#define CUSTOM_SETTINGS //Permet de ne charger que les fonctions des shields utilisés (afin d'alléger la taille du programme une fois compilé)
#define INCLUDE_CAMERA_SHIELD
#define INCLUDE_LCD_SHIELD
#define INCLUDE_SMS_SHIELD
#define INCLUDE_EMAIL_SHIELD
#define INCLUDE_PHONE_SHIELD

/* LISTE DES BIBLIOTHEQUES UTILISES */
#include <OneSheeld.h> //Inclus la bibliothèque OneSheeld afin d'utiliser ses fonctions spécifiques


/* DEFINITION DES N° DE PINS */
const int button_pin = 2; //Constante contenant le n° du pin relié au bouton simulant l'alerte lors de la détection d'un intrus
const int keySwitch_pin = 3; //Constante contenant le n° du pin relié au commutateur à clef permettant de réactiver le robot
const int red_pin = A13; //Constante contenant le n° du pin relié à la broche rouge de la led RGB
const int green_pin = A14; //Constante contenant le n° du pin relié à la broche verte de la led RGB
const int blue_pin = A15; //Constante contenant le n° du pin relié à la broche bleue de la led RGB

/* PARAMETRES : COORDONNEES DU PROPRIETAIRE */
const String phoneNumber = "0612345678"; //Numéro de téléphone du propriétaire
const String phoneMessage = "Une intrusion a eu lieu dans votre entrepôt !"; //Contenu du SMS
const String emailAdress = "monadresse@email.com"; //Email du propriétaire
const String emailSubject = "[Entrepôt] Alerte intrusion"; //Objet de l'Email
const String emailBody = "Une intrusion a eu lieu dans votre entrepôt ! (voir photo ci-jointe)"; //Contenu de l'Email

/* DECLARATION DE VARIABLES */
int keySwitch_state; //Variable contenant l'état du commutateur à clef
int button_state; //Variable contenant l'état du bouton simulant l'alerte
String robot_state; //Variable contenant l'état du robot


void setup()
{
  /* DEFINITION DES MODES DE PINS */
  pinMode(button_pin, INPUT); //Pin du bouton = ENTREE
  pinMode(keySwitch_pin, INPUT); //Pin du commutateur à clef = ENTREE
  pinMode(red_pin, OUTPUT); //Pin de la broche rouge de la led RGB = SORTIE
  pinMode(green_pin, OUTPUT); //Pin de la broche verte de la led RGB = SORTIE
  pinMode(blue_pin, OUTPUT); //Pin de la broche bleue de la led RGB = SORTIE

  /* INITIALISATION DE VARIABLES */
  keySwitch_state = 1; //Initialisation de l'état du commutateur à clef
  button_state = 1; //Initialisation de l'état du bouton simulant l'alerte
  robot_state = "OK"; //Initialisation de l'état du robot

  /* CONNEXION CARTE ONESHEELD+ */
  OneSheeld.begin(); //On commence à utiliser la carte OneSheeld+

  /* INITIALISATION DE L'ETAT DU ROBOT */
  OneSheeld.delay(4000);
  LCD.begin(); //On commence à utiliser le shield LCD
  LCD.blink(); //Active le curseur clignottant
  LCD.print(robot_state); //On affiche l'état du robot
  ledRGB(0, 255, 0); //LED = VERTE
}


void loop() {
  if (robot_state == "OK") { /* SI L'ETAT DU ROBOT EST "OK" */
  //--- DEBUT ETAT "OK"
    button_state = digitalRead(button_pin); //Lecture de l'état du bouton simulant l'alerte
    if (button_state == 0) { //Si on détecte une alerte
      new_robot_state("Alerte"); //Appel de la fonction changeant l'état du robot
      ledRGB(255, 0, 0); //LED = ROUGE
    }
  //--- FIN ETAT "OK"
  } else if (robot_state == "Alerte") { /* SI L'ETAT DU ROBOT EST "Alerte" */
  //--- DEBUT ETAT "Alerte"
    new_robot_state("Traitement"); //Appel de la fonction changeant l'état du robot
    ledRGB(255, 255, 255); //LED = BLANCHE
  //--- FIN ETAT "Alerte"
  } else if (robot_state == "Traitement") { /* SI L'ETAT DU ROBOT EST "Traitement" */
  //--- DEBUT ETAT "Traitement"
    OneSheeld.delay(4000);
    SMS.send(phoneNumber, phoneMessage); //Envoie du SMS

    OneSheeld.delay(4000);
    Camera.setQuality(HIGH_QUALITY); //Choix de la qualité de photo (ici qualité élevée)
    Camera.frontCapture(); //Prend une photo avec la caméra frontale
    OneSheeld.delay(4000);
    Email.attachLastPicture(emailAdress, emailSubject, emailBody, 0); //Envoie l'email contenant la photo

    OneSheeld.delay(4000);
    Phone.call(phoneNumber); //Appel téléphonique

    new_robot_state("Standby"); //Appel de la fonction changeant l'état du robot
    ledRGB(0, 0, 255); //LED = BLEUE
  //--- FIN ETAT "Traitement"
  } else if (robot_state == "Standby") { /* SI L'ETAT DU ROBOT EST "Standby" */
  //--- DEBUT ETAT "Standby"
    keySwitch_state = digitalRead(keySwitch_pin); //Lecture de l'état du commutateur à clef permettant de réactiver le robot
    if (keySwitch_state == 0) { //Si le commutateur à clef est actionné (= s'il y a une impulsion)
      new_robot_state("OK"); //Appel de la fonction changeant l'état du robot
      ledRGB(0, 255, 0); //LED = VERTE
    }
  //--- FIN ETAT "Standby"
  }
}


/* FONCTION DU CHANGEMENT D'ETAT DU ROBOT */
void new_robot_state(String new_state) {
  robot_state = new_state; //Changement d'état du robot
  OneSheeld.delay(4000);
  LCD.clear(); //On efface l'état précédent
  LCD.print(robot_state); //On affiche le nouvel état du robot
  OneSheeld.delay(2000); //Pause de 2s marquant le changement d'état du robot
}

/* FONCTION DU CHANGEMENT DE COULEUR DE LA LED RGB */
void ledRGB(int R, int G, int B) {
  analogWrite(red_pin, R); //On envoie une valeur entre 0 et 255 pour la couleur Rouge
  analogWrite(green_pin, G); //On envoie une valeur entre 0 et 255 pour la couleur Verte
  analogWrite(blue_pin, B); //On envoie une valeur entre 0 et 255 pour la couleur Bleue
}

