#!/bin/bash

#nom et version de l'application
APP_NAME="mbash"
APP_VERSION="1.0.0"

# Emplacement des fichiers binaires
BIN_DIR="bin"

# Emplacement du dépôt local
REPO_DIR="mbash"

# Distribution pour le dépôt
DISTRIBUTION="mydistribution"

# Création de l'arborescence de paquet
mkdir -p $mbash_${APP_VERSION}/DEBIAN

# Copie des fichiers binaires
cp ${BIN_DIR}/* ${APP_NAME}_${APP_VERSION}/

# Création du fichier control
echo "Package: ${APP_NAME}
Version: ${APP_VERSION}
Section: base
Priority: optional
Architecture: amd64
Depends:
Maintainer: Mathis SEILER <mathis.seiler8@etu.univ-lorraine.fr>
Description: My package description" > ${APP_NAME}_${APP_VERSION}/DEBIAN/control

# Création du paquet
dpkg-deb --build ${APP_NAME}_${APP_VERSION}

# Signe le paquet
dpkg-sig --sign builder ${APP_NAME}_${APP_VERSION}.deb

# Initialise le dépôt local
reprepro init

# Ajout du paquet au dépôt local
reprepro includedeb ${DISTRIBUTION} ${APP_NAME}_${APP_VERSION}.deb

