// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file


#ifndef TEXT_WCONS_H_INCL
#define TEXT_WCONS_H_INCL

#include "../text.h"

namespace too
{
	namespace gui
	{
		class WCONSText : public IText
		{
		public:
			WCONSText(const Point& p, const std::string& s) : IText(p, s) {}
			// default copy and assignment ok

		protected:
			virtual void draw_details() const;
		};
	}
}

#endif
