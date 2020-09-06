#ifndef __BILIBILI_H
#define __BILIBILI_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

extern String NAME ;  //改成自己的名字
extern String QiUID ;  //改成自己的UID 19577966  435162639 483818980
extern String MyUID  ;
extern String QiollowerUrl ;   // 粉丝数
extern String MyFollowerUrl ;   // 粉丝数
extern String ViewAndLikesUrl ; // 播放数、点赞数
extern String WeatherUrl ;   // 时间日期

extern long Follower ;   // 粉丝数
extern long View ;       // 播放数
extern long Likes ;      // 获赞数
extern int WeatherCode;
extern int Temperature;

void getViewAndLikes(String url);
int getFollower(String url);
void getWeather(String url);

#endif 

