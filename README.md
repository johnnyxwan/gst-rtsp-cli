# gst-rtsp-cli
Simple Command-line Interface RTSP Server, powered by GStreamer  
Similar with https://gitlab.freedesktop.org/gstreamer/gst-rtsp-server/-/blob/master/examples/test-launch.c  
Added features:  
1. Custom binding ip address  
2. Multiple endpoints support  

Okay, in human language: Want to turn "USB Webcam + Raspberry PI" into an IP Camera? Or need to delivery audio from Raspberry PI to elsewhere over the network (using RTSP)? This project can be what you need.  

Future plan:  
1. Release pre-compiled binaries  
2. Add more stdout and stderr output  
3. Create Node.JS binding and start RTSP server easily in Node.JS  
4. More platform and protocol support.  

Contributions are welcome.  

## Usage
This project started as a Raspberry PI project, so the follow instructions are only tested on Raspberry PI OS. Feel free to test on other Debian based system or translate them for other system. Contributions are welcome as well.  

1. Install Dependencies  
```
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install gstreamer1.0-tools
sudo apt-get install gstreamer1.0-plugins-good gstreamer1.0-plugins-ugly
```
Depends on application, other plugins might be needed in your particular pipeline.  
Here are some commonly used packages:  
gstreamer1.0-plugins-bad, gstreamer1.0-alsa, gstreamer1.0-libav, gstreamer1.0-rtsp  

2. Download Pre-compiled Binaries  
TODO  
If you preferred compile the source code yourself, please refer to the Build section.  

3. Start the Server  
The command format is:  
```
./gst-rtsp-cli [-p PORT] [-b BIND_ADDRESS] ENDPOINT1 LAUNCHLINE1 [ENDPOINT2 LAUNCHLINE2]...
```
Examples:  
```
./gst-rtsp-cli "/video" "videotestsrc ! x264enc ! rtph264pay name=pay0 pt=96"
./gst-rtsp-cli -p 8000 "/video" "videotestsrc ! x264enc ! rtph264pay name=pay0 pt=96"
./gst-rtsp-cli -p 8000 -b 127.0.0.1 "/video" "videotestsrc ! x264enc ! rtph264pay name=pay0 pt=96"
./gst-rtsp-cli "/video" "videotestsrc ! x264enc ! rtph264pay name=pay0 pt=96" "/audio" "audiotestsrc ! lamemp3enc ! rtpmpapay name=pay0 pt=97"
```

## Build
This project started as a Raspberry PI project, so the follow instructions are only tested on Raspberry PI OS. Feel free to test on other Debian based system or translate them for other system. Contributions are welcome as well.  

1. Install Dependencies  
```
sudo apt-get update
sudo apt-get upgrade
sudo apt-get install libgstrtspserver-1.0-dev
sudo apt-get install gstreamer1.0-plugins-good gstreamer1.0-plugins-ugly
```
libgstrtspserver-1.0-dev is required to build the project.  
gstreamer1.0-plugins-good includes rtph264pay.  
gstreamer1.0-plugins-base (dependency of gstreamer1.0-plugins-good) includes videotestsrc.  
gstreamer1.0-plugins-ugly includes x264enc.  
Install required plugins if you need anything else.  

2. Obtain Source Code  
```
git clone https://github.com/johnnyxwan/gst-rtsp-cli.git
cd gst-rtsp-cli
```

3. Compile  
```
make
```

4. Execute Compiled Binaries  
```
./dist/gst-rtsp-cli "/video" "videotestsrc ! x264enc ! rtph264pay name=pay0 pt=96"
```

5. Edit Source Code  
The source code is in src folder. After updating, do Step 3 and 4 again.  
