gst-rtsp-cli : src/main.c
	gcc src/main.c -o dist/gst-rtsp-cli `pkg-config --cflags --libs gstreamer-rtsp-server-1.0 gstreamer-1.0`

clean :
	rm -f dist/*
