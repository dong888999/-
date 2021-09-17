/*
 * w_DefScrollWindow.h
 *
 *  Created on: 2021年8月23日
 *      Author: Admin
 */

#ifndef JNI_SCROLLWINDOW_W_DEFSCROLLWINDOW_H_
#define JNI_SCROLLWINDOW_W_DEFSCROLLWINDOW_H_

#include "system/Thread.h"
#include "control/ZKBase.h"
#include "utils/Log.h"
#include <cmath>


#define WZ20

#ifdef WZ6
#include "utils/VelocityTracker.h"
#endif

#ifdef WZ20
#include "VelocityTracker.h"
#endif


#define NOCYCLE

/*     两种轮播方式只能使用一种,需要定义相应的宏声明               */
#if defined NOCYCLE            //启用不循环轮播方式
#elif defined CYCLE			   //启用循环轮播方式
#endif


#define DEF_SCROLLROLL_SPEED			10    ///屏幕自动翻滚速度
#define TURN_SCROLLPAGE_MIN_SPEED		0.0001 /// 大于等于移速,改变页码

struct WPOINT{
	int x;
	int y;

	WPOINT(int sX=0,int sY=0):x(sX),y(sY){}

	WPOINT& operator=(const WPOINT& sWPOINT){
		this->x = sWPOINT.x;
		this->y = sWPOINT.y;
		return *this;
	}
};



#endif /* JNI_SCROLLWINDOW_W_DEFSCROLLWINDOW_H_ */
