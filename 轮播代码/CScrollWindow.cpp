/*
 * CScrollWindow.cpp
 *
 *  Created on: 2021年6月24日
 *      Author: Admin
 */

#include "CScrollWindow.h"

#define WFILE "CScrollWindow.cpp"

#include <math.h>

CScrollWindow::CScrollWindow() :
	mIsRolling(false),
	mCurRollSpeed(DEF_SCROLLROLL_SPEED),
	mIndexlist(0),
	mRollSpeed(DEF_SCROLLROLL_SPEED),
	mStartDownTime(0),
	mScrollPageChangeListenerPtr(NULL) {
	// TODO 自动生成的构造函数存根

	mIsCanMove = false;
}

CScrollWindow::~CScrollWindow() {
	// TODO 自动生成的析构函数存根
}

void CScrollWindow::resetPage(){
	mIsCanMove = false;
	mIsRolling = false;
	mIndexlist = 0;
	mStartDownTime = 0;
	mRollSpeed = DEF_SCROLLROLL_SPEED;
	mCurRollSpeed = DEF_SCROLLROLL_SPEED;
	mScrollPageChangeListenerPtr  = NULL;

	mControlsList.clear();
}

void CScrollWindow::setScrollPageChangeListener(IScrollPageChangeListener *pListener) {
	mScrollPageChangeListenerPtr = pListener;
}

//添加的窗口，只有mLeft不同
void CScrollWindow::addPage(ZKBase *pPage,const LayoutPosition posDefault) {
	if (NULL == pPage){
		LOGD("\n\n\n#######:%s	(%d),impossible\n",__FILE__,__LINE__);
		return;
	}
	if (!mControlsList.empty()) { // 不是空的  需要移动
		const LayoutPosition &lastPos = mControlsList.back()->getPosition();    // 获取最后一个控件的坐标
		LayoutPosition pos = pPage->getPosition();                              // 获取新加控件的坐标
		pos.mLeft = lastPos.mLeft + lastPos.mWidth;
		pPage->setPosition(pos);                                		// 新加控件的左边坐标，位于最后一个控件的右侧
	} else {
		pPage->setPosition(posDefault);                         		// 如果新加控件为第一个控件，控件坐标设置为自定义的坐标值
		mPosition = pPage->getPosition();                    			// 保存第一个控件的坐标
	}
	mControlsList.push_back(pPage);             						// 将新加控件，加到控件链表
}

void CScrollWindow::setPageShow(const int pageNum){

}

int CScrollWindow::getPageSize() const {
	return mControlsList.size();                              				//获取页面数
}


#if defined NOCYCLE
//判断页码是否到达边界           //   " 不循环方式  "
bool CScrollWindow::setPageSize()
{
	if (mIndexlist < 0 ){
		mIndexlist = 0;
		return true;
	}
	else if (mIndexlist > getPageSize() -1){
		mIndexlist = getPageSize() -1;
		return true;
	}
	return false;
}

//移动函数      	  		//  " 不循环方式   "
void CScrollWindow::movePage(int dx) {

	if (getpagesize() < 2)  //优化
     return;

	LayoutPosition lp = mControlsList[mIndexlist]->getPosition(),      // 获取控件坐标(1)
			lpVice = mControlsList[mIndexlist]->getPosition();         // 获取控件坐标(2)

	if (lp.mLeft == dx) {                  					 //位置从0开始，则不需要移动
		return;
	}

	// 移动 左边的一页窗口
	if (mIndexlist > 0 ) {           // 不是第一页

		lp = mControlsList[mIndexlist - 1]->getPosition();       // 获取左边一页的坐标
		lp.mLeft = -mPosition.mWidth + dx;                   // 左边界从 -mWidth 移动
		mControlsList[mIndexlist - 1]->setPosition(lp);       // 设置左控件的坐标
	}
	// 移动右边的一页窗口
	if (mIndexlist < getPageSize() - 1) {                         // 不是最后一页

		lp = mControlsList[mIndexlist + 1]->getPosition();       // 获取右边一页的坐标
		lp.mLeft = mPosition.mWidth + dx;                        // 左边界从 mWidth 移动
		mControlsList[mIndexlist + 1]->setPosition(lp);			 // 设置右控件的坐标
	}

// 通用滑动系数      约  1/3 宽度处                         具体精度与  mIsCanMove = true 的设置距离有关
	int k = lpVice.mWidth/9;

	// 移动当前的一页
	if (0 == mIndexlist && dx > 0)    					//第一页的右滑
	{
		if (dx <= k)
		{
			lpVice.mLeft = dx;                  //							    /  x ;        (0 < x <= k)
		}										//  触摸滑动距离  与 窗口移动距离的关系    y = |
		else									//                              \ 根号(kx);   (k < x )
		{
			lpVice.mLeft = sqrt(k*(dx));
		}

	}
	else if(getPageSize()-1 == mIndexlist && dx < 0)    //最后一页的左滑
	{
		if (dx >= -k)
		{
			lpVice.mLeft = dx;
		}
		else
		{
			lpVice.mLeft = -sqrt(k*(-dx));
		}
	}
	else
	{
		lpVice.mLeft = dx;
	}
	//lpVice.mLeft = dx;
	LOGD("dx = %d\n",dx);
	mControlsList[mIndexlist]->setPosition(lpVice);
}

#elif defined CYCLE
//判断页码是否到达边界           //  " 循环方式   "
bool CScrollWindow::setPageSize()
{
	if (mIndexlist < 0 ){

		mIndexlist = getPageSize() -1;
		return true;
	}
	else if (mIndexlist > getPageSize() -1){
		mIndexlist = 0;
		return true;
	}
	return false;
}

// 移动函数                            //  " 循环方式   "
void CScrollWindow::movePage(int dx) {

	if (getpagesize() < 2)       // 页面在 2页 及以上时，进行移动窗口
     return;

	LayoutPosition lp = mControlsList[mIndexlist]->getPosition(),      // 获取控件坐标(1)
			lpVice = mControlsList[mIndexlist]->getPosition();         // 获取控件坐标(2)

	if (lp.mLeft == dx) {                  					 //位置从0开始，则不需要移动
		return;
	}

	// 移动 左边的一页窗口
	if (mIndexlist > 0) {                        				 // 不是第一页

		lp = mControlsList[mIndexlist - 1]->getPosition();       // 获取左边一页的坐标
		lp.mLeft = -mPosition.mWidth + dx;                   // 左边界从 -mWidth 移动
		mControlsList[mIndexlist - 1]->setPosition(lp);       // 设置左控件的坐标
	}
	// 移动右边的一页窗口
	if (mIndexlist < getPageSize() - 1) {                         // 不是最后一页

		lp = mControlsList[mIndexlist + 1]->getPosition();       // 获取右边一页的坐标
		lp.mLeft = mPosition.mWidth + dx;                        // 左边界从 mWidth 移动
		mControlsList[mIndexlist + 1]->setPosition(lp);			 // 设置右控件的坐标
	}


	// 做成循环滑动的效果
	// 移动 左边的一页窗口
	if (mIndexlist == 0) {                                       // 第一页

		int mCurrentPageTmp = getPageSize() - 1;                 // 最后一页
		lp = mControlsList[mCurrentPageTmp]->getPosition();      // 获取最后一页的坐标
		lp.mLeft = -mPosition.mWidth + dx;                       // 左边界从  -mWidth 移动
		mControlsList[mCurrentPageTmp]->setPosition(lp);		 // 设置最后一页的坐标
	}
	// 移动右边的一页窗口
	if ((mIndexlist == getPageSize() - 1)) {                      // 最后一页

		int mCurrentPageTmp = 0;                                  // 第一页
		lp = mControlsList[mCurrentPageTmp]->getPosition();       // 获取第一页的坐标
		lp.mLeft = mPosition.mWidth + dx;                         // 左边界从  mWidth 移动
		mControlsList[mCurrentPageTmp]->setPosition(lp);          // 设置第一页的坐标
	}

	// 移动当前的一页
	lpVice.mLeft = dx;
	mControlsList[mIndexlist]->setPosition(lpVice);
}
#endif

void CScrollWindow::notifyScrollPageChange() {                   // 报告轮播页面发生改变
	if (mScrollPageChangeListenerPtr != NULL) {
		mScrollPageChangeListenerPtr->onScrollPageChange(this, mIndexlist);  // 记录轮播类对象，页面编号
	}
}

// 判断可不可以滑动
bool CScrollWindow::needToRoll() const {

	if (!mControlsList.empty()) {
		return mControlsList[mIndexlist]->getPosition().mLeft != 0;      //控件边界未到达0，就需要滑动
	}

	return false;
}

// 通过滑动判断左移还是右移动
bool CScrollWindow::onTouchEvent(const MotionEvent &ev) {
	if (mControlsList.empty() || mIsRolling || isRunning()) {    //控件列表为空       正在滚动    线程在运行

		return false;
	}
	switch (ev.mActionStatus) {      //  ev.mActionStatus != E_ACTION_NONE     //发生了动作

		case MotionEvent::E_ACTION_DOWN:        		   // 按下
			//if((*SeekBar2)->getPosition())
			if (mPosition.isHit(ev.mX, ev.mY)) {      //在控件范围内是否点击
				mDownPoint.x = ev.mX;
				mDownPoint.y = ev.mY;
				mIsCanMove = false;
				mStartDownTime = clock();                // 保存按下的时间
			}
			break;

		case MotionEvent::E_ACTION_MOVE: {               // 移动
			if (!mIsCanMove) {

				if(abs(ev.mX - mDownPoint.x) > 10){           // 滑动距离绝对值大于  10               可以 移动状态为真
					mIsCanMove = true;

					mDownPoint=WPOINT(ev.mX,ev.mY);       // 滑动距离10后，开始算移动距离        /******** 用来保证这 10 距离不会直接跳跃 ******/

				}     										//此段代码不加，轮播效果将会 0 距离生效
				return false;
			}

			int dx = ev.mX - mDownPoint.x;               // 移动距离
			movePage(dx);                   		 	 //进行移动

			// 用于计算滑动速度
			mVelocityTracker.addMovement(ev);

			break;
		}

		case MotionEvent::E_ACTION_UP:
		case MotionEvent::E_ACTION_CANCEL: {  // 抬手判断 向左滑动还是向右滑动
			mUpPoint.x = ev.mX;
			mUpPoint.y = ev.mY;         // 记录抬起坐标

			// 用于计算滑动速度
			mVelocityTracker.addMovement(ev);

			float speed = mVelocityTracker.getXVelocity();  // 获取y移动速度

			if ((mUpPoint.x - mDownPoint.x > mPosition.mWidth / 4) ||          // 滑动距离   > 宽度1/4  ||  滑动速度大于最小虚度
				(speed > TURN_SCROLLPAGE_MIN_SPEED)) {
				mCurRollSpeed = mRollSpeed;                // 移速
			} else if ((mUpPoint.x - mDownPoint.x < -mPosition.mWidth / 4) ||       // 反向运动
				(speed < -TURN_SCROLLPAGE_MIN_SPEED)) {
				mCurRollSpeed = -mRollSpeed;
			} else {
				mCurRollSpeed = ((mUpPoint.x - mDownPoint.x > 0) ? -mRollSpeed : mRollSpeed);      //  回滚速度与方向
			}

			mVelocityTracker.reset();
			if (needToRoll()) {   // 可以运行就启动计时器
				mIsRolling = true;

				if(!isRunning()) {
					run();
					//LOGD("!isRegistered\n");
				}
			}

			break;
		}
		case MotionEvent::E_ACTION_NONE:
		default:
			break;
	}

	return false;
}

/**
 * @brief 线程循环调用该接口
 * @return true 不退出线程，false 将退出线程
 */
bool CScrollWindow::threadLoop(){
	sleep(5);

	if (needToRoll()) {                                  	  //  能够滑动
		int curPage = mIndexlist;  //获取页码
		LayoutPosition lp = mControlsList[curPage]->getPosition();   //获取当前页面的坐标

		int dx = lp.mLeft + mCurRollSpeed;          /******    左边界 + 当前移速     *****/

		if (mCurRollSpeed > 0)      // 向右滚动
		{

#if defined NOCYCLE
			if (lp.mLeft < 0 || 1 == mControlsList.size() || 0 == curPage) 	  //  不循环模式回滚
#elif defined CYCLE
			if (lp.mLeft < 0 || 1 == mControlsList.size())	 				  //  循环模式回滚
#endif
			{
				if (dx > 0)
				{
					dx = 0;
				}

			}
			else
			{
				if (dx > mPosition.mWidth)
				{
					dx = 0;
					//向右翻页
					--mIndexlist;

					setPageSize();
					notifyScrollPageChange();
				}
			}
		}
		else 						// 向左滚动
		{
#define MLEFT mLeft
#if defined NOCYCLE
			if (lp.MLEFT > 0  || 1 == mControlsList.size() || getPageSize()-1 == curPage)   // 不循环模式回滚
#elif defined CYCLE
			if (lp.mLeft > 0  || 1 == mControlsList.size())  							    // 循环模式回滚
#endif
			{
				if (dx < 0)
				{
					dx = 0;
				}

			}
			else
			{
				if (dx < -mPosition.mWidth)
				{
					dx = 0;
					++mIndexlist;

					setPageSize();
					notifyScrollPageChange();
				}
			}
		}
		movePage(dx);

	}
	if (!needToRoll())
	{
		mIsRolling = false;
		//刷新页面，标数，
		return false;
	}
	return true;
}
