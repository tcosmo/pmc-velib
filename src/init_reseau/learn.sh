ite=$1
if [ -z $1 ]; then
    ite=10000
fi

if [ -z $(ls | grep learning.log) ]; then
    echo "Teaching...";
    ../../teacher $(pwd) $ite
else
    echo "Apprentissage déjà effectué"
fi
