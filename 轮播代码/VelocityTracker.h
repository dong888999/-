/*
 * VelocityTracker.h
 *
 *  Created on: Jul 19, 2017
 *      Author: guoxs
 */

#ifndef _UTILS_VELOCITY_TRACKER_H_
#define _UTILS_VELOCITY_TRACKER_H_

#include "control/Common.h"
//typedef struct _POINT_
//{
//    /**
//     * The x coordinate of the point.
//     */
//    int x;
//    /**
//     * The y coordinate of the point.
//     */
//    int y;
//}POINT;
typedef struct _POINT
{
    /**
     * The x coordinate of the point.
     */
    int x;
    /**
     * The y coordinate of the point.
     */
    int y;
} POINT;
#define ID_SCROLLWINDOW_ROLL_TIMER 208

class VelocityTracker {
public:
	VelocityTracker();
	virtual ~VelocityTracker();

	void addMovement(const MotionEvent &ev);
	void reset();

	float getXVelocity() const;
	float getYVelocity() const;

private:
	bool canComputeVelocity() const;

private:
	int mCurIndex;
	MotionEvent mEvents[2];
};

#endif /* _UTILS_VELOCITY_TRACKER_H_ */
