#pragma once

namespace myengine {
	
	class Stream
	{
	public:
			//Stream();
			virtual ~Stream();

		virtual bool		isValid() const = 0;
		virtual unsigned	get(unsigned char *data, unsigned size) = 0;
		virtual void		rewind() = 0;
	};

}
