
CHEMINP=scheme-FFCM1_Red.dat
THERMINP=thermdat.dat
FINALFILE=FFCM1_Red.cpp

FMC=${PELE_PHYSICS_HOME}/Support/Fuego/Pythia/products/bin/fmc.py
HEADERDIR=${PELE_PHYSICS_HOME}/Support/Fuego/Mechanism/Models/header

${FUEGO_PYTHON} ${FMC} -mechanism=${CHEMINP} -thermo=${THERMINP} -name=${FINALFILE}

echo Compiling ${FINALFILE}...
