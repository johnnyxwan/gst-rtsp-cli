#include <gst/gst.h>
#include <gst/rtsp-server/rtsp-server.h>

#define DEFAULT_RTSP_PORT "8554"
#define DEFAULT_RTSP_ADDRESS "0.0.0.0"

static char *port = (char *) DEFAULT_RTSP_PORT;
static char *address = (char *) DEFAULT_RTSP_ADDRESS;

static GOptionEntry entries[] = {
  {"port", 'p', G_OPTION_FLAG_NONE, G_OPTION_ARG_STRING, &port, "Port to listen on (default: " DEFAULT_RTSP_PORT ")", "PORT"},
  {"bind-address", 'b', G_OPTION_FLAG_NONE, G_OPTION_ARG_STRING, &address, "Bind server to address (default: " DEFAULT_RTSP_ADDRESS ")", "ADDRESS"},
  {NULL}
};

int main(int argc, char *argv[]) {
  GMainLoop *loop;
  GstRTSPServer *server;
  GstRTSPMountPoints *mounts;
  GOptionContext *optctx;
  GError *error = NULL;

  /* Parse command line options */
  optctx = g_option_context_new("ENDPOINT1 LAUNCHLINE1 [ENDPOINT2 LAUNCHLINE2]...\n\n"
    "Example: \"/video\" \"videotestsrc ! x264enc ! rtph264pay name=pay0 pt=96\" \"/audio\" \"audiotestsrc ! lamemp3enc ! rtpmpapay name=pay0 pt=97\"");
  g_option_context_set_summary(optctx, "Simple Command-line Interface RTSP Server, powered by GStreamer, with multiple endpoints support");
  g_option_context_add_main_entries(optctx, entries, NULL);
  g_option_context_add_group(optctx, gst_init_get_option_group());
  if(!g_option_context_parse(optctx, &argc, &argv, &error)) {
    g_printerr("Error parsing options: %s\n", error->message);
    g_option_context_free(optctx);
    g_clear_error(&error);
    return -1;
  }
  g_option_context_free(optctx);
  if(argc < 2) {
    g_printerr("Missing endpoint and launchline options\n");
    return -1;
  }
  if(argc % 2 != 1) {
    g_printerr("Endpoint and launchline options come in pairs\n");
    return -1;
  }

  /* Create a server instance, and get the mounts object reference */
  loop = g_main_loop_new(NULL, FALSE);
  server = gst_rtsp_server_new();
  gst_rtsp_server_set_service(server, port);
  gst_rtsp_server_set_address(server, address);
  mounts = gst_rtsp_server_get_mount_points(server);

  /* Create endpoints */
  for(int i = 1; i < argc; i += 2) {
    if(gst_rtsp_mount_points_match(mounts, argv[i], NULL) != NULL) {
      g_print("Endpoint %s repeated, skip...\n", argv[i]);
      continue;
    }
    GstRTSPMediaFactory *factory = gst_rtsp_media_factory_new();
    gst_rtsp_media_factory_set_launch(factory, argv[i + 1]);
    gst_rtsp_media_factory_set_shared(factory, TRUE);
    gst_rtsp_mount_points_add_factory(mounts, argv[i], factory);
    g_print("Creating endpoint rtsp://%s:%s%s (%s)\n", address, port, argv[i], argv[i + 1]);
  }

  /* Clean up */
  g_object_unref(mounts);

  /* Start server */
  gst_rtsp_server_attach(server, NULL);
  g_print("RTSP Server Ready");
  g_main_loop_run(loop);

  /* Normal return */
  return 0;
}
