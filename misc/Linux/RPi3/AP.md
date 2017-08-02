# Raspberry Pi 3 Access Point

This document summarizes the process to enable a Raspberry Pi 3 (RPi3)
to become an access point.  This is a pre-requsite for
the [the Local Cloud](https://github.com/spark/spark-server).

# References

https://cdn-learn.adafruit.com/downloads/pdf/setting-up-a-raspberry-pi-as-a-wifi-access-point.pdf
https://www.raspberrypi.org/documentation/configuration/wireless/access-point.md
https://frillip.com/using-your-raspberry-pi-3-as-a-wifi-access-point-with-hostapd/
http://blog.ithasu.org/2016/10/using-a-raspberry-pi-3-as-a-wifi-access-point-and-bridge/

# Platform

## Hardware

Raspberry Pi 3

## Software

Raspbian Jessie July 5, 2017

# General notes

Don’t set up dnsmasq, it does not work for whatever reason on the RPi3.
Use isc-dhcp-server.

Most of the reference documentation insist that you need a network USB
dongle to turn a RPi3 into an AP.  The documentation here uses the
native on-board wireless card.

You may use `sudo` from a regular account or apply all these changes as 
root.

# Reconfigure network interfaces

Configure the network: eth0 wlan0

You can use any network configuration you desire.  You
do not need to use the settings given in this tutorial.

This tutorial uses the subnet 10.1.0.1 to 10.1.0.254.
DHCP uses a subset of this range.

## Edit: /etc/network/interfaces

auto lo eth0

allow-hotplug wlan0  
iface wlan0 inet static  
    address 10.1.0.1
    netmask 255.255.255.0
#wpa-conf /etc/wpa_supplicant/wpa_supplicant.conf

iface eth0 inet dhcp

# Restart the wlan0 port

```
ifdown wlan0
ifup wlan0
```

# DHCP client (dhcpcd)

We do not want the DCHP client to service anything on wlan0.

## Edit: /etc/dhcpcd.conf

```
denyinterfaces wlan0
```

## Restart the dhcpcd service

```
systemctl restart dhcpcd
```

# DHCP server (dhcpd)

## Install

```
apt-get install isc-dhcp-server
```

## Edit: /etc/dhcp/dhcpd.conf

```
ddns-update-style none;
#option domain-name "example.org";
#option domain-name-servers ns1.example.org, ns2.example.org;
default-lease-time 600;
max-lease-time 7200;
authoritative;
subnet 10.1.0.0 netmask 255.255.255.0 {
  range 10.1.0.2 10.1.0.20;
  option broadcast-address 10.1.0.255;
  option routers 10.1.0.1;
  default-lease-time 600;
  max-lease-time 7200;
  option domain-name-servers 10.0.1.1;
}
```

## Restart dhcpd

The regular service calls do not seem to work very well.  Use
`ps` to find the process and kill it.   Then, use the service
calls to restart it and verify that it is running.

When in doubt reboot!

# Install and configure (hostapd)

WARNING: Choose your own `ssid` and `password` other than the ones below.  Do not use quotes!

## Install hostapd

```
apt-get install hostapd
systemctl stop hostapd
```

## Edit: /etc/hostapd/hostapd.conf

```
# This is the name of the WiFi interface we configured above
interface=wlan0

# Use the nl80211 driver with the brcmfmac driver
driver=nl80211

# This is the name of the network
ssid=rpinet

# Use the 2.4GHz band
hw_mode=g

# Use channel 6
channel=6

# Enable 802.11n
ieee80211n=1

# Enable WMM
wmm_enabled=1

# Enable 40MHz channels with 20ns guard interval
ht_capab=[HT40][SHORT-GI-20][DSSS_CCK-40]

# Accept all MAC addresses
macaddr_acl=0

# Use WPA authentication
auth_algs=1

# Require clients to know the network name
ignore_broadcast_ssid=0

# Use WPA2
wpa=2

# Use a pre-shared key
wpa_key_mgmt=WPA-PSK

# The network passphrase
wpa_passphrase=raspberry

# Use AES, instead of TKIP
rsn_pairwise=CCMP
```

## Edit: /etc/default/hostapd

```
DAEMON_CONF="/etc/hostapd/hostapd.conf"
```

# Start hostapd service

```
service hostapd start
```

# Optional: Internet connected to eth0

If you have Internet available on eth0 and want to allow your AP clients to access the Internet
use the following instructions.

## Make a system change

Editing the sysctl.conf will allow the setting to be applied for each reboot.

### Edit /etc/sysctl.conf

```
net.ipv4.ip_forward=1
```

### Apply the change now

```
echo 1 > /proc/sys/net/ipv4/ip_forward 
```

## Install 

```
apt-get install iptables-persistent
```

## Configure the firewall 

Setup forwarding of wlan0 packets to eth0.

### Run 

```
iptables -t nat -A POSTROUTING -o eth0 -j MASQUERADE
iptables -A FORWARD -i eth0 -o wlan0 -m state --state RELATED,ESTABLISHED -j ACCEPT
iptables -A FORWARD -i wlan0 -o eth0 -j ACCEPT
```

## Save changes the firewall

```
sh -c "iptables-save > /etc/iptables/rules.v4"
```

# Reboot and test

For the real test, reboot to get a clean slate.  Then, try out your
new access point.   If things are working as you like, we are now
ready to try out the local cloud software.

# Troubleshooting

Ask for for help on the Particle community forum.
