
#include "filter_pushdown_constants.hpp"

// Note: Only set these flags
bool filter_pushdown_flag = false;
bool filter_fusion_flag = true;
bool scan_buffering_flag = true;

// Note: Should be less than 0.5 to have any benefits
double caching_threshold = 0.3;


// NOTE: DO NOT TOUCH THESE
bool disable_filter_pushdown = !filter_pushdown_flag;
bool disable_filter_fusion = (disable_filter_pushdown)? true: !filter_fusion_flag;
bool disable_scan_buffering = !scan_buffering_flag;
