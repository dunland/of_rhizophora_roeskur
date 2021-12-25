# Röskur Visuals

creates visuals from recorded audio tracks using purr-data and openframeworks.  
All audio tracks serve as toggles and send a trigger signal to of via OSC/UDP.

## Running the app

1. start the openframeworks app via `of_rhizophora_röskur$ make RunRelease`
2. run the purr-data sketch by klicking "start". If `udpsend` says "not connected", connect manually in `[osc_udp_send]`>`connect localhost 9001`


## Problems

### purr-data slowing down, many xruns on JACK

** trying to disable pulseaudio:**

``` bash
dav@dav-ubu:~$ killall -9 pulseaudio
dav@dav-ubu:~$ systemctl --user stop pulseaudio.socket
Warning: The unit file, source configuration file or drop-ins of pulseaudio.socket changed on disk. Run 'systemctl --user daemon-reload' to reload units.
dav@dav-ubu:~$ systemctl --user stop pulseaudio.service
Warning: The unit file, source configuration file or drop-ins of pulseaudio.service changed on disk. Run 'systemctl --user daemon-reload' to reload units.
dav@dav-ubu:~$ systemctl --user stop pulseaudio.socket
Warning: The unit file, source configuration file or drop-ins of pulseaudio.socket changed on disk. Run 'systemctl --user daemon-reload' to reload units.
dav@dav-ubu:~$ systemctl --user stop pulseaudio.service
Warning: The unit file, source configuration file or drop-ins of pulseaudio.service changed on disk. Run 'systemctl --user daemon-reload' to reload units.
dav@dav-ubu:~$ systemctl --user disable pulseaudio.socket
dav@dav-ubu:~$ systemctl --user disable pulseaudio.service
dav@dav-ubu:~$ systemctl --user mask pulseaudio.socket
Created symlink /home/dav/.config/systemd/user/pulseaudio.socket → /dev/null.
dav@dav-ubu:~$ systemctl --user mask pulseaudio.service
Created symlink /home/dav/.config/systemd/user/pulseaudio.service → /dev/null.
dav@dav-ubu:~$ pkill pulseaudio 
```