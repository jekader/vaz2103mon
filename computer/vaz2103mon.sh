#!/bin/bash
# Status script for wmfs
# based on https://bbs.archlinux.org/viewtopic.php?pid=872150#p872150

ttyport="/dev/ttyUSB0"
echo "initializing $ttyport"
#initialize port
stty -F $ttyport 9600 cs8 -cstopb
exec 3<>$ttyport
echo "beginning to send data"
while :
do

sleep 1

    # Collect system information 
    MEM=$(awk '/Mem/ {print $3}' <(free -m))
    # CPU line courtesy Procyon: https://bbs.archlinux.org/viewtopic.php?pid=661592
    CPU=$(eval $(awk '/^cpu /{print "previdle=" $5 "; prevtotal=" $2+$3+$4+$5 }' /proc/stat); sleep 0.4; 
         eval $(awk '/^cpu /{print "idle=" $5 "; total=" $2+$3+$4+$5 }' /proc/stat); 
         intervaltotal=$((total-${prevtotal:-0})); 
         echo "$((100*( (intervaltotal) - ($idle-${previdle:-0}) ) / (intervaltotal) ))")
     HD=$(awk '/^\/dev/{print $5}' <(df -P))
#    PCM=$("$HOME/Scripts/pacman-up.pl")
    INT=$(host google.com>/dev/null && echo $GRN"ON" || echo $RED"NO")
    DTE=$(date "+%I:%M")

# write to Serial
echo $CPU $RAM $NET
ARDCPU="$(($CPU*2+60)),0,0"
echo  $ARDCPU >&3
echo $ARDCPU
#head -n 1 $ttyport

done

# close port
exec 3>&-
