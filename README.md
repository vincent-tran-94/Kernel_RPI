# Projet fin de formation POEI Data Scientest

## Contexte 
Au sein de ce projet de fin de parcours dédié à la formation en développement système sous
Linux, l'accent est mis sur l'application pratique des compétences acquises tout au long du
cursus. <br>
Dans ce contexte, l'objectif est de mettre en pratique les connaissances en développement
système, en programmation noyau et en gestion des périphériques, en créant un système
fonctionnel basé sur une carte Raspberry Pi 3 (RPi). <br> Cette carte sera intégrée à un réseau et
interagira avec divers capteurs, actionneurs et périphériques, offrant ainsi aux participants
l'occasion de démontrer leur maîtrise des concepts essentiels du développement système. <br>

## Description du projet 
Ce projet de fin de parcours vise à concevoir un système dédié à la surveillance et à la
gestion d'une carte Raspberry Pi 3 (RPi) connectée à divers capteurs et actionneurs. <br>
Ce système sera accessible via une machine du réseau fonctionnant sous Linux. <br>
La RPi sera équipée d'un écran LCD 16x2 connecté via le bus I2C, ainsi que d’une diode LED,
d'un bouton poussoir et d'un capteur de température DHT11. L'interaction avec ces
périphériques sera gérée par des modules noyau. <br>
Les principales fonctionnalités du système comprennent :
• Affichage sur l'écran LCD d'un message d'accueil personnalisable par l'utilisateur,
ainsi que de la température capturée par le capteur DHT11. Les données de
température seront également enregistrées dans des fichiers journaux. <br>
• Possibilité pour l'utilisateur d'activer ou de désactiver le clignotement de la diode
LED via l'application ou en utilisant le bouton poussoir connecté à la RPi. <br>
• Acquisition de la température ambiante grâce au capteur de température. <br>
• Mise en place d'un programme exécuté sur la Raspberry Pi pour assurer la
communication entre l'application de supervision et la RPi. <br>
L'application de contrôle et de supervision permettra de : <br>
• Modifier la fréquence de capture des données de température. <br>
• Consulter la dernière valeur de température capturée. <br>
• Accéder à l'historique des valeurs de température. <br>
• Personnaliser le message d'accueil affiché sur l'écran LCD. <br>
