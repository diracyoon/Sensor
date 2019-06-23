#!/bin/sh
### BEGIN INIT INFO
# Provides:          Environmental Sensor
# Required-Start:
# Required-Stop:
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
### END INIT INFO

stop() {
    echo "kill Sensor"
    killall Sensor
}

start() {
    echo "start Sensor"
    /home/iyoon/Sensor/Build/Sensor
}

case "$1" in
    start)
	start
	;;
    stop)
	stop
	;;
    restart)
	stop
	start
	;;
    *)
	echo "Usage  $0  {start|stop|restart}"
esac
