#!/bin/bash
# Status script for wmfs
# based on https://bbs.archlinux.org/viewtopic.php?pid=872150#p872150

ttyport="/dev/ttyUSB0"

# the dials on the instrument panel are as follows:
# бензин вода  масло тахометр   спидометр
#
# gas    water oil   tachometer speedometer
#
# sizes
#
# small  small small large      large
#
# driven by
#
# PWM    PWM   PWM   PWM?       stepper
#
# data
#
# I/O    temp  net   RAM        CPU
#
# Arduino pins
# ?      ?     9     10          11 
# 6      11    10    9         2,3,4,5


# define ranges for PWM
# CPU -> large stepper controlled dial
CPUMIN=0
CPUMAX=500
PREVCPUz=0
# RAM -> large PWM controlled dial
RAMMIN=0
RAMMAX=200
# net -> small PWM controlled dial
NETMIN=85
NETMAX=0
# temp -> small PWM controlled dial
TEMPMIN=100
TEMPMAX=0
# I/O -> small PWM controlled dial
IOMIN=100
IOMAX=0
#define max bandwidth and interface to monitor (I use a DSL router so link speed doesn't give much)
net_max_downlink_mbps=40
uplink_if=br0

# define conversion factor
#CPUf=$((($CPUMAX-$CPUMIN)/100))
CPUf=$( echo "scale=1; ($CPUMAX-$CPUMIN)/100" | bc)
RAMf=$( echo "scale=1; ($RAMMAX-$RAMMIN)/100" | bc)
NETf=$( echo "scale=1; ($NETMAX-$NETMIN)/100" | bc)
TEMPf=$( echo "scale=1; ($TEMPMAX-$TEMPMIN)/100" | bc)
IOf=$( echo "scale=1; ($IOMAX-$IOMIN)/100" | bc)
echo "constants:  CPUf: $CPUf RAMf: $RAMf NETf: $NETf TEMPf: $TEMPf IOf: $IOf"
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
ARDCPU="-100,$RAMMIN,$NETMIN,$TEMPMIN,$IOMIN"
echo  $ARDCPU >&3
sleep 5

echo -n "."
# all 100%
ARDCPU="10,$RAMMAX,$NETMAX,$TEMPMAX,$IOMAX"
echo  $ARDCPU >&3
sleep 2

done
echo "done"
############## end tests

echo "sending actual data"
while :
do

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

oldbytes=$(cat /sys/class/net/$uplink_if/statistics/rx_bytes )
# sleep between measurements
sleep 1
newbytes=$(cat /sys/class/net/$uplink_if/statistics/rx_bytes )
# bytes received in a second

diffbytes=$( echo "$newbytes-$oldbytes" | bc)
# converting to mbps
diffmegabits=$( echo "$diffbytes*8/(1024*1024)" | bc)
NET=$( echo "$diffmegabits*100/$net_max_downlink_mbps" | bc)
if [ "$NET" -gt "100" ]; then
NET=100
fi
#echo "net debug: $newbytes - $oldbytes = $diffbytes bytes > $diffmegabits mbps > $NET %"

# write to Serial
echo "RAW:" $PREVCPUz $CPU $RAM $NET
CPUzi=$(echo "$CPU*$CPUf - $PREVCPUz"|bc)
CPUz=$(  echo $CPUzi | awk '{print int($1)}' )
PREVCPUz=$(echo "$CPU*$CPUf"|bc)

RAMzi=$(echo "$RAMMIN+$RAM*$RAMf"|bc)
RAMz=$(  echo $RAMzi | awk '{print int($1)}' )

NETzi=$(echo "$NETMIN+$NET*$NETf"|bc)
NETz=$(  echo $NETzi | awk '{print int($1)}' )

ARDCPU="$CPUz,$RAMz,$NETz,0,0"
echo  $ARDCPU >&3
echo "SND:" $ARDCPU
echo -n "RCV:"
head -n 1 $ttyport

done

# close port
exec 3>&-
