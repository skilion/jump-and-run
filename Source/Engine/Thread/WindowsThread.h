#pragma once
#ifndef __WINDOWS_THREAD_H__
#define __WINDOWS_THREAD_H__


#include "Thread.h"
#include "System/WindowsHeader.h"


namespace myengine {

	class WindowsThread: public Thread
	{
	public:
			WindowsThread(Runnable *runnable, bool autoDelete);
			//~WindowsThread();

		virtual bool	isActive();

		virtual void	start();
		virtual void	kill();

		virtual void	join();
		virtual void	join(unsigned milliseconds);


	private:
		void			closeHandle();
		static void		entryPoint(WindowsThread *thread);

		HANDLE			hThread;
	};

}


#endif //!__WINDOWS_THREAD_H__
