# cmts
transcode system


# Plan
1. Environment build scripts
2. Transcode node init version
3. Transcode scheduler init version
4. Http API init version
5. Support live streaming output [HTTP+TS, HTTP+FLV, RTMP, HLS]

# Build
1. bash install.sh

# Build Transcode Node
1. cd src/
2. make

# Start Transcode Node
1. ./bin/node


# Test Use telnet command:

 # telnet 127.0.0.1 8899
 Trying 127.0.0.1...
 Connected to localhost.
 Escape character is '^]'.
 ffmpeg -v quiet -i ~/Movies/objectC/facebook.mp4 -f mpegts -y /tmp/output.ts
 waiting pid=78905



