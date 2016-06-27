#include "runperf.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <cmath>

using namespace std;

runperf::runperf()
{
  weightchange = 0.0;
}

runperf::runperf(float t, float d)
{
  runtime = t;
  distance = d;
  weightchange = 0.0;
}

void runperf::pace_calc()
{
  pace_calc(runtime, distance);
}
void runperf::pace_calc(float runtime, float distance)
{
  pace = runtime / distance;
}

float runperf::get_pace()
{
  return pace;
}


void runperf::VO2max_calc()
{
  VO2max_calc(runtime, distance);
}
void runperf::VO2max_calc(float t, float d)
{
  /* VO2max according to Jack Daniels formula */
  
  float speed_m_min = d*1000/t;
  float VO2 = -4.6+0.182258*speed_m_min+0.000104*pow(speed_m_min,2);
  float percent_max = 0.8+0.1894393*exp(-0.012778*t)      \
    + 0.2989558*exp(-0.1932605*t);
  VO2max = VO2/percent_max;
  VO2max = VO2max * (1.0 + weightchange); // Weight adjusted
}


float runperf::get_VO2max()
{
  return VO2max;
}


void runperf::predict(float d, float w)
{

  weightchange = w;
  distance_new = d;
  predtime = pace*distance_new;
  float predtime_lo = predtime*0.5;
  float predtime_hi = predtime*2.0;

  float VO2max;
  float VO2max_pred;
  float delta;
  
  VO2max = get_VO2max();
  VO2max_calc(predtime, distance_new);
  delta = abs(get_VO2max() - VO2max);

  int i = 0;
  while (delta > pow(10,-4)) {
    VO2max_calc(predtime, distance_new);
    VO2max_pred = get_VO2max();
    
    if (VO2max_pred < VO2max) {
      predtime_hi = predtime;
    } else {
      predtime_lo = predtime;
    }
    predtime = (predtime_lo + predtime_hi)/2.0;
    delta = abs(VO2max_pred - VO2max);
  }
  
}

float runperf::get_predpace()
{
  float predpace = predtime/distance_new; 
  return predpace;
}

float runperf::get_predtime()
{
  return predtime;
}