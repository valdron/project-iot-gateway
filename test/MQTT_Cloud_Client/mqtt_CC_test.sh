topicTemperature="Roboterarm/Temperature"
topicPressure="Roboterarm/Pressure"
topicState="Roboterarm/State"
anzahlPubs=10



for ((z=0;z<$anzahlPubs;z++))
do
    tempValue=$((RANDOM %3+40))
    presValue=$((RANDOM %3+1))
    stateValue=$((RANDOM%10))
    mosquitto_pub -t $topicTemperature -m $tempValue
    mosquitto_pub -t $topicPressure -m $presValue
    mosquitto_pub -t $topicState -m $stateValue
done
echo "Test Done"