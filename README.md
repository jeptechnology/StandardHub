# StandardHub Workshop

This repo is designed to help with the workshop to be held on June 2nd 2021.

## Prerequisites:

- Standard Hub
- Interface board
- USB serial cable (FTDI)
- Hub is configured with Internet access

## Steps: 

Find your hub's IP address either:
- `ip a` on your serial console to Linux
- check with your router

Log on to your hub using ssh:
`ssh fio@[hub_ip_address]`

Go to your home folder and clone this repo:
```
cd ~
git clone https://github.com/jeptechnology/StandardHub.git
```

Workaround for docker-compose as we are not registering with the foundries cloud:
```
cd ~/StandardHub
sudo ./prepare.sh
```

Now build and launch the docker compose app 
```
cd ~/StandardHub/Docker
docker-compose up --build
```

The sandbox container is now up and running and listening for SSH connectiones on port 2222.
This container is one that you can develop your code inside.
Connect to this container from your development PC:

```
ssh -p 2222 fio@[hub_ip_address]
```

You need to clone this repo again inside the sandbox container. I recommend you install to `/data/shared` so that it will be preserved if you were to update your container in future.
```
cd /data/shared
git clone https://github.com/jeptechnology/StandardHub.git
```

Build the HelloWorld example:

```
cd /data/shared/StandardHub/HelloWorld
cmake .
make
./HelloWorld
```

Try experimenting with LEDs in the sandbox container:

```
echo "255 1000 255 1000" > /hardware/leds/status_red
echo "255 500 0 500" > /hardware/leds/status_green
echo "255 100 0 200" > /hardware/leds/status_blue

echo "0 1000 0 1000" > /hardware/leds/status_red
echo "0 1000 0 1000" > /hardware/leds/status_green
echo "0 1000 0 1000" > /hardware/leds/status_blue
```

Now try building and running the LED blink example to see this controlled from code:

```
cd /data/shared/StandardHub/BlinkLED
cmake .
make
./BlinkLED
```

Ctrl-C to stop.

Try the other examples: 

- SetupButton
- EchoServer (for read/writing with UART 2 on the interface board)

