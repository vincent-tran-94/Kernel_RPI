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
Les principales fonctionnalités du système comprennent : <br>
• Affichage sur l'écran LCD d'un message d'accueil personnalisable par l'utilisateur,
ainsi que de la température capturée par le capteur DHT11. Les données de
température seront également enregistrées dans des fichiers journaux. <br>
• Possibilité pour l'utilisateur d'activer ou de désactiver le clignotement de la diode
LED via l'application ou en utilisant le bouton poussoir connecté à la RPi. <br>
• Acquisition de la température ambiante grâce au capteur de température. <br>
• Mise en place d'un programme exécuté sur la Raspberry Pi pour assurer la
communication entre l'application de supervision et la RPi. <br>
L'application de contrôle et de supervision permettra de : <br>
• Consulter la dernière valeur de température capturée. <br>
• Accéder à l'historique des valeurs de température. <br>
• Personnaliser le message d'accueil affiché sur l'écran LCD. <br>

## Etapes d'installation pour faire la compilation du noyau
### Installation de chaine de compilation croisée
`sudo apt update`
`sudo apt full-upgrade`

Installer les paquets suivants :
`sudo apt install build-essential gawk gcc g++ gfortran git texinfo bison
libncurses-dev libssl-dev`

Décompresser le fichier téléchargé .tar.gz avec la commande suivante :
`tar xf cross-gcc-10.3.0-pi_64.tar.gz`

Déplacer le répertoire contenant les fichiers décompressés dans le répertoire /opt avec la commande suivante :
`sudo mv cross-gcc-10.3.0-pi_64/ /opt/cross-pi64`

Ajouter les deux lignes suivantes dans le fichier .bashrc qui se trouve dans le répertoire de connexion :
`export PATH=/opt/cross-pi64/bin:$PATH`
`export LD_LIBRARY_PATH=/opt/cross-pi64/lib:$LD_LIBRARY_PATH`

`lsmod` : lister les modules actifs
`insmod` <module.ko> : charger le module module.ko
`rmmod` <nom module> : décharger un module
`modinfo` <module.ko> : afficher les informations relatifs à un module
`modprobe` : gérer un modules placé dans le répertoire /lib/modules/$(uname -r)

Pour afficher les messages de debug des modules :
`dmesg`

### Recompilation du noyau pour la Raspberry Pi 

Créer un répertoire pour re-compiler le noyau :
`mkdir rpi3b64 && cd rpi3b64/`
Télécharger les fichiers sources du noyau :

~/rpi3b64$ 
`git clone --depth=1 -b rpi-6.1.y https://github.com/raspberrypi/linux`

Creer un script shell compile.sh dans le dossier ~/rpi3b64/linux et l’exécuter chaque fois pour préparer
une session de développement pour les modules noyau :

`export KERNEL=kernel8`
`make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- bcm2711_defconfig`
`make -j4 ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- Image modules dtbs`

Une fois la compilation est effectuée, on procède par mettre à jour les nouveaux fichiers créés dans la carte SD
contenant le Raspberry OS. Pour cela, mettre la carte SD sur la machine hôte, et créer le script copy.sh dans le
dossier ~/rpi3b64/linux :

`mkdir -p ~/mnt/fat32 ~/mnt/ext4`
`sudo mount /dev/mmcblk0p1 ~/mnt/fat32`
`sudo mount /dev/mmcblk0p2 ~/mnt/ext4/`
`sudo env PATH=$PATH make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- INSTALL_MOD_PATH=~/mnt/ext4 modules_install`
`sudo cp ~/mnt/fat32/kernel8.img ~/mnt/fat32/kernel8-backup.img`
`sudo cp arch/arm64/boot/Image ~/mnt/fat32/kernel8.img`
`sudo cp arch/arm64/boot/dts/broadcom/*.dtb ~/mnt/fat32/`
`sudo cp arch/arm64/boot/dts/overlays/*.dtb* ~/mnt/fat32/overlays/`
`sudo cp arch/arm64/boot/dts/overlays/README ~/mnt/fat32/overlays/`
`sudo umount ~/mnt/fat32`
`sudo umount ~/mnt/ext4`
`rm -rf ~/mnt/fat32 ~/mnt/ext4`




