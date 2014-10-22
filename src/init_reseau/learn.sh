if [ -z $(ls | grep learning.log) ]; then
    echo "Teaching...";
    ../../teacher $(pwd)
else
    echo "Apprentissage déjà effectué"
fi
