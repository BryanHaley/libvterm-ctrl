PORT=/dev/tty.usbmodem14121
BAUD=9600

make flash
echo
screen $PORT $BAUD
