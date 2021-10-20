#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <unistd.h>
#include <mosquitto.h>

using namespace std;

constexpr auto MQTT_WISER_HOME = "wiser-home";   // use this topic prefix to send to wiser heat

struct mosquitto *mosq;

static void connect_callback(struct mosquitto *mosq, void *obj, int result)
{
   printf("[MQTT] connect callback, rc=%d\n", result);
}

static void message_receive_callback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message)
{  
   printf("[MQTT] rx for topic '%s':\n", message->topic);
}

static void publish_heartbeat_message()
{
   auto topic = "heartbeat";
   string data = "{}"; //empty JSON
   printf("[MQTT] publishing to %s\n", topic);

   mosquitto_publish(mosq, NULL, topic, data.length(), data.c_str(), 0, false);
}

int main(int argc, char *argv[])
{
   char clientid[] = "MQTT_Example";
   int rc = 0;

   mosquitto_lib_init();

   mosq = mosquitto_new(clientid, true, 0);

   if (mosq != nullptr)
   {
      mosquitto_connect_callback_set(mosq, connect_callback);
      mosquitto_message_callback_set(mosq, message_receive_callback);
   }

   while (mosq != nullptr)
   {      
      auto mqtt_host  = "mqtt";  // NOTE: This is the "hostname" of the mosquitto docker container
      auto mqtt_port  = 1883;
      auto keep_alive = 60;

      // Connection to MQTT bus
      while (mosquitto_connect(mosq, mqtt_host, mqtt_port, keep_alive))
      {
         printf("MQTT failed to connect to %s:%d - retrying in 10 seconds\n", mqtt_host, mqtt_port);
         sleep(5);
      }

      // Process communications on MQTT bus, wait 2000ms
      auto timeout_ms = 2000;
      while(0 == (rc = mosquitto_loop(mosq, timeout_ms, 1)))
      {        
         publish_heartbeat_message();
      }

      // 
      printf("MQTT connection error - retrying in 10 seconds\n");
      mosquitto_disconnect(mosq);
      sleep(5);
   }

   mosquitto_lib_cleanup();

   return rc;
}
