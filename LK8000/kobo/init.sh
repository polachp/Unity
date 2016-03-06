#wait for /dev/ttymxc0 available ( timeout after 10sec )
x=0
while [ "$x" -lt 100 -a ! -e /dev/ttymxc0 ]; do
   x=$((x+1))
   sleep .1
done
#!/bin/sh
###########################################################
# init tty
###########################################################
. /mnt/onboard/LK8000/kobo/setTTY.sh

###########################################################
# init CONFIGURATION
###########################################################
sleep 2
. /mnt/onboard/LK8000/kobo/unityConfig.sh
