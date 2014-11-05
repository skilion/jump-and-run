#pragma once
#ifndef __THREAD_H__
#define __THREAD_H__


namespace myengine {

	class Runnable
	{
	public:
		virtual void	run() = 0;
	};

	class Thread
	{
	public:
			Thread(Runnable *runnable, bool autoDelete);
			virtual ~Thread();

		virtual bool	isActive() = 0;

		virtual void	start() = 0;
		virtual void	kill() = 0;

		virtual void	join() = 0;
		virtual void	join(unsigned milliseconds) = 0;

		static void		sleep(unsigned milliseconds);
		static void		yield();


	protected:
		bool		autoDelete;		//If true the object must destroy itself on thread exit
		Runnable	*runnable;
	};

	Thread		*createThread(Runnable *runnable, bool autoDelete);

}


#endif //!__THREAD_H__
