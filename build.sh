#!/bin/bash

# Garde en tête le répertoire courant
CWD="$(pwd)"

# Fonction pour lancer le programme
RunProgram() {
    cd $CWD
    ./run.sh
}

# Décide si oui ou non lancer le programme
EndCompilation() {
    if [ "$1" = "ON" ] 
    then
        RunProgram
    fi
}

ShowHelp() {
    echo "$(basename "$0") [-h] <args...>  : Script de compilation de LegacyOfTheGrasdoux"
    echo ""
    echo "-h : Affiche ce message"
    echo ""
    echo "Liste des arguments :"
    echo "- help    : Affiche ce message"
    echo "- clean   : Supprime tous les fichiers temporaires de build et les exécutables avant la compilation"
    echo "- doc     : Compile la documenation avec Doxygen"
    echo "- release : Compile le programme en mode \"release\" (sans warnings, toutes optimisations)"
    echo "- debug   : Compile le programme en mode \"debug\" (tout warnings, aucune optimisations, hot reload)"
    echo "- run     : Lance l'exécutable nouvellement compilé si la compilation a réussi"
}

# Affiche l'aide si besoin
if [ "$1" = "help" ] || [ "$1" = "-h" ]
then
    ShowHelp
    exit 0
fi


# Garde en tête les arguments de ligne de commande
BUILD_AND_RUN="OFF"
OLD_CMAKELISTS_CONTENTS="$(cat CMakeLists.txt)"
for ARGUMENT in "$@"
do
    case "$ARGUMENT" in
        run)
            BUILD_AND_RUN="ON" ;;
        release)
            sed -i "s/set(CMAKE_BUILD_TYPE [A-Za-z]*)/set(CMAKE_BUILD_TYPE Release)/" CMakeLists.txt ;;
        debug)
            sed -i "s/set(CMAKE_BUILD_TYPE [A-Za-z]*)/set(CMAKE_BUILD_TYPE Debug)/" CMakeLists.txt ;;
        doc*)
            sed -i "s/set(COMPILE_DOXYGEN OFF)/set(COMPILE_DOXYGEN ON)/" CMakeLists.txt ;;
        clean)
            ./clean.sh build bin ;;
        *) 
            echo "Argument invalide : \"$ARGUMENT\"" >&2 ; ShowHelp ; exit 1
    esac
done

# Compile le programme (chaque étape ne tourne que si la précédente a réussi)
cmake CMakeLists.txt -B ./build -S ./ && cd build && make -j && EndCompilation "$BUILD_AND_RUN"

# On remet le répertoire courant comme défini en haut du script
cd $CWD

# On remet le contenu du CMakeLists.txt comme il l'était avant
echo "$OLD_CMAKELISTS_CONTENTS" > CMakeLists.txt