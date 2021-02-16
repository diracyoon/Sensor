#!/bin/sh
### BEGIN INIT INFO
# Provides:          Environmental Monitor
# Required-Start:
# Required-Stop:
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
### END INIT INFO

stop() {
    echo "kill Env_Monitor"
    killall Env_Monitor
}

start() {
    echo "start Env_Monitor"
    /home/iyoon/Env_Monitor/Build/Env_Monitor
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
