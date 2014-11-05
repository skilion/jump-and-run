#pragma once
#ifndef __LINUX_THREAD_H__
#define __LINUX_THREAD_H__


#include <pthread.h>

#include "Thread.h"


namespace myengine {

	class LinuxThread: public Thread
	{
	public:
			LinuxThread(Runnable *runnable, bool autoDelete);
			//~LinuxThread();

		virtual bool	isActive();

		virtual void	start();
		virtual void	kill();
		virtual void	join();
		virtual void	join(unsigned milliseconds);

		//void			closeHandle();


	private:
		static void		entryPoint(LinuxThread *thread);

		pthread_t		pThread;
	};

}


#endif //!__LINUX_THREAD_H__
