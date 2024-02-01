#include "main.h"


int main(int argc, char **argv)
{
  ros::init(argc, argv, "fast_lio_sam_sc_qn_node");
  ros::NodeHandle nh_private("~");

  FastLioSamScQnClass fast_lio_sam_sc_qn_(nh_private);

  ros::AsyncSpinner spinner(4); // Use multi threads
  spinner.start();
  ros::waitForShutdown();

  fast_lio_sam_sc_qn_.~FastLioSamScQnClass(); // Explicit call of destructor
 
  return 0;
}