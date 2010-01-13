#!/bin/bash

### BEGIN INIT INFO
# Provides: networking
# Required-Start: mountkernfs $local_fs
# Required-Stop: $local_fs
# Should-Start: ifupdown
# Should-Stop: ifupdown
# Default-Start: S
# Default-Stop: 0 6
# Short-Description: Raise network interfaces
### END INIT INFO

. /lib/lsb/init-functions

case "$1" in
	start)
		log_action_begin_msg "Configuring network interfaces"
		ifup auto
		log_action_end_msg $?
		;;
	stop)
		log_action_begin_msg "Deconfiguring network interfaces"
		ifdown all
		log_action_end_msg $?
		;;
	restart|force-reload)
		$0 stop
		$0 start
		;;
	*)
		echo "E: usage: $0 stop|start|restart|force-reload" >&2
		;;
esac
