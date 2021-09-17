/*
 * CScrollWindow.h
 *
 *  Created on: 2021年6月24日
 *      Author: Admin
 */

#ifndef JNI_W_FILE_SCROLLWINDOW_CSCROLLWINDOW_H_
#define JNI_W_FILE_SCROLLWINDOW_CSCROLLWINDOW_H_

#include "w_DefScrollWindow.h"

// 轮播类
class CScrollWindow:public Thread {
public:
	CScrollWindow();
	virtual ~CScrollWindow();

public:/// serprop

	/// 初始化轮播类
	void resetPage();

	/// 添加的窗口，设置默认位置
	void addPage(ZKBase *pPage,const LayoutPosition posDefault);

	/// 触摸处理
	bool onTouchEvent(const MotionEvent &ev);

	/// 设置第 "pageNum" 页显示
	void setPageShow(const int pageNum);

public:/// getprop

	/// 获取轮播页数量
	int getPageSize() const;

private:
	class IScrollPageChangeListener {    //********//		//理解为      轮播触发监听类
	public:
		virtual ~IScrollPageChangeListener() { }
		virtual void onScrollPageChange(CScrollWindow *pScrollWindow, int page) = 0;
	};

	/**
	 * @brief 线程循环调用该接口
	 * @return true 不退出线程，false 将退出线程
	 */
	virtual bool threadLoop();
private:/// getprop

	void setScrollPageChangeListener(IScrollPageChangeListener *pListener);

	// 判断页码是否到达边界 如果是 则给出相反的极值
	bool setPageSize();

	// 移动函数
	void movePage(int dx);

private:/// setprop

	void notifyScrollPageChange();

	// 判断可不可以滑动
	bool needToRoll() const;
private:
	bool mIsRolling;
	bool mIsCanMove;
	int mCurRollSpeed;
	int mIndexlist;				/// 控件列表的索引
	uint32_t mRollSpeed;
	long mStartDownTime;

	IScrollPageChangeListener *mScrollPageChangeListenerPtr;     // 滑动翻页监听
	WPOINT mDownPoint;
	WPOINT mUpPoint;
	VelocityTracker mVelocityTracker;       // 速度追踪
	std::vector<ZKBase *> mControlsList;    /// 控件列表
	LayoutPosition mPosition;
};

#endif /* JNI_W_FILE_SCROLLWINDOW_CSCROLLWINDOW_H_ */
