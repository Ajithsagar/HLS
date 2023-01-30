#include <gst/gst.h>
#include<stdio.h>
int main(int argc, char *argv[]) {
  GstElement *pipeline;
  GstBus *bus;
  GstMessage *msg;
  GstStateChangeReturn ret;

  /* Initialize Gstreamer */
  gst_init(&argc, &argv);

  /* Create pipeline */
  pipeline = gst_parse_launch("filesrc location=output1.ts ! tsdemux ! decodebin ! videoconvert ! autovideosink ", NULL);

  /* Start pipeline */
  ret = gst_element_set_state(pipeline, GST_STATE_PLAYING);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    g_printerr("Unable to set pipeline to the playing state.\n");
    gst_object_unref(pipeline);
    return -1;
  }

  bus = gst_element_get_bus(pipeline);
  msg = gst_bus_timed_pop_filtered(bus,GST_CLOCK_TIME_NONE,GST_MESSAGE_ERROR | GST_MESSAGE_EOS);

  /* Wait for pipeline to finish */
  ret = gst_element_get_state(pipeline, NULL, NULL, GST_CLOCK_TIME_NONE);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    g_printerr("Pipeline failed.\n");
  } else {
    g_print("Pipeline completed.\n");
  }

  /* Clean up */
  gst_object_unref(bus);
  gst_object_unref(msg);
  gst_element_set_state(pipeline,GST_STATE_NULL);
  gst_object_unref(pipeline);
  return 0;
}
