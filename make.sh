function clear() {
    rm ./*.exe
}

function advice() {
    echo "Usage: "
    echo "   ./make.sh (<help> / <clear>) (optional)"
}

if [ "$1" = "clear" ]; then
    clear
    exit
elif [ "$1" = "help" ]; then
    advice
    exit
fi

gcc main.c -Wall -o main