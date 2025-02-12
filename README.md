# Smart Floorheating control

# Using Rasp Zero W

### uname -a:

Linux raspberrypi 5.10.63+ #1459 Wed Oct 6 16:40:27 BST 2021 armv6l GNU/Linux

### lscpu: 

Architecture:        armv6l
Byte Order:          Little Endian
CPU(s):              1
On-line CPU(s) list: 0
Thread(s) per core:  1
Core(s) per socket:  1
Socket(s):           1
Vendor ID:           ARM
Model:               7
Model name:          ARM1176
Stepping:            r0p7
CPU max MHz:         1000.0000
CPU min MHz:         700.0000
BogoMIPS:            697.95
Flags:               half thumb fastmult vfp edsp java tls


## Confiuration

Put a connectionhomeassistant.json with thw following input to "etc/smartfloorheating/connectionhomeassistant.json"

```JSON
{
  "api": {
    "token": "eyJhbXXXXXXXXXXXXXXXXXXXXXLONGLIFETOKEN",
    "ip": "192.168.x.xx",
    "port": "8123"
  },
  "temperaturesensor": {
    "entity_id": {
      "TempBad": "sensor.tempbad_temperature",
      "TempKind1": "sensor.tempkind1_temperature",
      "TempKueche": "sensor.tempkueche_temperature",
      "TempSchlafzimmer": "sensor.tempschlafzimmer_temperature",
      "TempWohnzimmer": "sensor.tempwohnzimmer_temperature"
    }
  },
  "thermostat": {
    "entity_id": {
      "bad": "climate.bad",
      "ankleidezimmer": "climate.ankleidezimmer",
      "kueche": "climate.kueche",
      "schlafzimmer": "climate.schlafzimmer",
      "wohnzimmer": "climate.wohnzimmer"
    }
  },
  "heater": {
    "entity_id": {
      "bad": "input_boolean.heater_bad",
      "ankleidezimmer": "input_boolean.heater_kind1",
      "kueche": "input_boolean.kueche",
      "schlafzimmer": "input_boolean.schlafzimmer",
      "wohnzimmer": "input_boolean.wohnzimmer"
    }
  }
}
```

## Toolchain Cross building

[Toolchain Cross building](https://github.com/tttapa/docker-arm-cross-toolchain)

## External lib needed

- Boost property tree, option
- curl
- openssl

### Install Lib

```bash
apt install libboost-all-dev pkg-config libssl-dev openssl libcurl4-openssl-dev

```

### bmc2835

Download it on the remote target and build it! After that copy it to the chroot

```bash
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.75.tar.gz
tar -zxvf bcm2835-1.75.tar.gz
cd bcm2835-1.75
./configure
make
sudo make check
sudo make install
```

### Cross Build

install the lib to the chroot:

- /usr/local/include/etl/ /var/lib/schroot/chroots/rpizero-bullseye-armhf/usr/local/include/
- /usr/local/share/etl/ /var/lib/schroot/chroots/rpizero-bullseye-armhf/usr/local/share/

to the target:

- /usr/local/share/etl/ /usr/local/share/
- /usr/local/include/etl/ /usr/local/include/

```bash
scp -r /usr/local/include/etl/ $user@$ip:/usr/local/include/
scp -r e/usr/local/share/etl/ $user@$ip:/usr/local/share/
```

## Build SmartfFloorHeating Projekt

```bash
cd SmartFloorHeating/ && cmake -DRASPBERRY_VERSION=1 -DPATH_INSTALLDIR=build -DCMAKE_BUILD_TYPE=Debug -DIPTARGET=<YOUR__DEVICE_IP> -DUSER=<YOUR_DEVICE_USER> -DCMAKE_TOOLCHAIN_FILE=cmake/crossarmv6l.cmake -DCMAKE_INSTALL_PREFIX=build -S . -B build -G Ninja 
```

```bash
cd build && ninja install
```

### Hardware
