#pragma once

#include <myString.h>

#include "Object.h"


namespace myengine {
	namespace gui {

		class Label: public Object
		{
		public:
			enum TextAlignment {
				ALIGN_LEFT,
				ALIGN_CENTER,
				ALIGN_RIGHT,
			};

			enum BorderType {
				BORDER_NONE,
				BORDER_SOLID
				//BORDER_EMBOSSED,
				//BORDER_IMPRESSED
			};

				Label(Container *parent, const String &text);
				//virtual ~Label();
			
			//Object interface
			virtual void	onChangeSize();
			virtual void	onDraw();
		
			const String	&getText() const;

			void			setText(const String &text);
			void			setTextAlignment(TextAlignment textAlignment);
			void			setBorderType(BorderType borderType);


		protected:
			void			updateTextPosition();

			String			text;
			TextAlignment	textAlignment;		
			BorderType		borderType;
			Vec2i			textPosition;
		};

	}
}
