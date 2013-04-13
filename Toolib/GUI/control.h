// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/** Abstract base class for all user controls, also called widgets.
*/
//! \file


#ifndef CONTROL_H_INCL
#define CONTROL_H_INCL

#include <vector>
#include <string>
#include "event.h"
#include "color.h"

namespace too
{
	namespace gui
	{
		class IShape;

		class IControl
		{
		public:
			explicit IControl(std::string sLabel = "") : m_parent(0), m_sLabel(sLabel), m_eventhandler(0) {}
			explicit IControl(IControl* parent, std::string sLabel = "")
				: m_parent(parent), m_sLabel(sLabel), m_eventhandler(0) {}
			virtual ~IControl()=0{}

			IControl* parent() { return m_parent; }
			IControl* window() { return m_parent; }

			void attach_eventhandler(fpEVENTHANDLERe eh) { m_eventhandler = eh; }

			void attachto(IControl* newparent) { m_parent = newparent; }
			void attach(IControl* c) { if (!c) return; c->attachto(this); m_controls.push_back(c); }

			void Label(const std::string & s) { m_sLabel = s; }
			const std::string& Label() { return m_sLabel; }

			virtual int x() const = 0;
			virtual int y() const = 0;
			virtual int w() const = 0;
			virtual int h() const = 0;
			virtual void show() = 0;
			virtual void hide() = 0;
			virtual bool visible() const = 0;
			virtual void activate() = 0;
			virtual void deactivate() = 0;
			virtual bool active() const = 0;

			virtual void datachange() = 0;
			virtual void dataunchange() = 0;
			virtual bool datachanged() const = 0;

			virtual void BgColor(const Color& c) = 0;
			virtual Color BgColor() const = 0;

			void attach(IShape* s) { m_shapes.push_back(s); }

			virtual void redraw();
		protected:
			virtual void draw() const;

			int handle_events(Event::EType e)
			{
				if (m_eventhandler)
					return m_eventhandler(e);
				else
					return 0;
			}
		private:
			IControl* m_parent;
			std::string m_sLabel;
			fpEVENTHANDLERe m_eventhandler;
			std::vector<IControl*> m_controls;
			std::vector<IShape*> m_shapes;

			// Better forbidden. Could/should be implemented with deep copy.
			IControl(const IControl&);
			IControl& operator=(const IControl&);
		};
	} // gui
} // too

#endif
