
//Festlegung des Persistencespeichers, der URI und der ClientID
// einzigartige ClientID gefordert
MqttClientPersistence persistence = new MqttDefaultFilePersistence("/tmp");
MqttClient client = new MqttClient("tcp://localhost:1883","MQTTSub",persistence);

MqttConnectOptions opts = new MqttConnectOptions();
//Keep Alive auf 480 Sekunden setzen
opts.setKeepAliveInterval(480);
//Quality of Service auf nach dem "Topic" auf 1 setzen für Acknowledgment
// --> später auf 0 , Behandlung wie Signal

opts.setWill(client.getTopic("WillTopic"),"Etwas unerwartetes ist Passiert! -Fehler!".getBytes(),1,true);
client.connect(opts);

// Die Nachricht wird immer als Byte-Array dargestellt !!!
MqttMessage msg = new MqttMessage("34°C".getBytes());
msg.setRetained(true);
// Quality of Service auf 2 -- höchste Persistence
msg.setQos(2);

// Senden einer Nachricht
// Topic getten !!
MqttTopic topic = client.getTopic("data/temperature");
MqttTopic topicZwei = client.getTopic("data/voltage");
// token wird verwendet zur Überprüfung ob Nachricht komplett versendet wurde
MqttDeliveryToken token = topic.publish(msg);


// Versende Nachrichten via CallBack
// Verwendet, Verbindungsabbruch oder Sendecomplete anzuzeigen
client.setCallback ( new MqttCallback() {
	public void messageArrived(MqttTopic topic, MqttMessage message)
		throws Exceoption{
			
		}
		public void deliveryComplete(MqttDeliveryToken token){
			
		}
		public void connectionLost(Throwable cause){
			
		}	
});
// nach dem Verbinden, Abonnieren mit Topic-String
client.subscribe("data/#");

//mehrere Topics zum selben Zeitpunkt - mehrere QoS-Level
client.subscribe(new String[]{"data/temperature","data/voltage"}, new int[]{2,0});

// stop von Nachrichtenempfang
client.unsubscribe(new String[]{"data/temperature","data/voltage")});

// Vorherige Sitzung fortsetzen
opts.setCleanSession(false);

//Verbindung trennen
client.disconnect();
