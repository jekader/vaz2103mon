#!/bin/bash
# Status script for wmfs
# based on https://bbs.archlinux.org/viewtopic.php?pid=872150#p872150

ttyport="/dev/ttyUSB1"

# define ranges for PWM
CPUMIN=100
CPUMAX=0
RAMMIN=85
RAMMAX=0
NETMIN=100
NETMAX=0

# define conversion factor
#CPUf=$((($CPUMAX-$CPUMIN)/100))
CPUf=$( echo "scale=1; ($CPUMAX-$CPUMIN)/100" | bc)
RAMf=$( echo "scale=1; ($RAMMAX-$RAMMIN)/100" | bc)
echo "constants:  $CPUf $RAMf "
echo "initializing $ttyport"
#initialize port
stty -F $ttyport 9600 cs8 -cstopb
exec 3<>$ttyport

####### test patterns

Cycles=3
echo "sending 0-100-0 patterns $Cycles times"
for Count in $(seq 1 $Cycles);
do

echo -n "$Count."
# all zeroes
ARDCPU="$CPUMIN,$RAMMIN,$NETMIN"
echo  $ARDCPU >&3
sleep 2

echo -n "."
# all 100%
ARDCPU="$CPUMAX,$RAMMAX,$NETMAX"
echo  $ARDCPU >&3
sleep 2

done
echo "done"
############## end tests

echo "sending actual data"
while :
do

sleep 1

    # Collect system information 
    RAM=$(awk '/Mem/ {print int(($3-$6-$7)*100/$2)}' <(free -m))
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
echo "RAW:" $CPU $RAM $NET
CPUzi=$(echo "$CPUMIN+$CPU*$CPUf"|bc)
CPUz=$(  echo $CPUzi | awk '{print int($1)}' )
RAMzi=$(echo "$RAMMIN+$RAM*$RAMf"|bc)
RAMz=$(  echo $RAMzi | awk '{print int($1)}' )
ARDCPU="$CPUz,$RAMz,0"
echo  $ARDCPU >&3
echo "SND:" $ARDCPU
echo -n "RCV:"
head -n 1 $ttyport

done

# close port
exec 3>&-
