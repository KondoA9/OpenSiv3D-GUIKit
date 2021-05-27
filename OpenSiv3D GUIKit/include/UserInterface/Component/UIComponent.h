#pragma once

#include "../../ColorTheme/DynamicColor.h"
#include "../MouseEvent/MouseEvent.h"
#include "../Layer/Layer.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class UIComponent {
	private:
		struct CallableMouseEvent {
			MouseEvent mouseEvent;
			Array<MouseEventHandler> handlers;
		};
		static Array<CallableMouseEvent> m_callableMouseEvents;

		Array<Layer*> m_dependentLayer;
		Array<MouseEventHandler> m_mouseEventHandlers;
		bool m_needToUpdateLayer = true;

	protected:
		bool m_mouseOver = false, m_preMouseOver = false;
		bool m_mouseDraggingEnable = false;

	public:
		ColorTheme backgroundColor;
		Layer layer;
		bool penetrateMouseEvent = false;

	public:
		UIComponent(const ColorTheme& _backgroundColor = DynamicColor::BackgroundSecondary) :
			backgroundColor(_backgroundColor)
		{}

		virtual ~UIComponent() = default;

		virtual void updateLayer();

		virtual bool updateLayerIfNeeded();

		virtual void draw() {};

		virtual void updateMouseEvent();

		void addEventListener(MouseEventType e, const std::function<void(const MouseEvent& e)>& f) {
			m_mouseEventHandlers.push_back(MouseEventHandler(e, f));
		}

		void addEventListener(MouseEventType e, const std::function<void()>& f) {
			m_mouseEventHandlers.push_back(MouseEventHandler(e, [f](const MouseEvent&) {f(); }));
		}

		void requestToUpdateLayer() {
			m_needToUpdateLayer = true;
		}

		void setConstraint(LayerDirection direction, UIComponent& component, LayerDirection toDirection, double constant = 0.0, double multiplier = 1.0);

		void setConstraint(LayerDirection direction, double constant = 0.0, double multiplier = 1.0);

		void setConstraint(LayerDirection direction, const std::function<double()>& func, double constant = 0.0, double multiplier = 1.0);

		void removeConstraint(LayerDirection direction);

		static void ResetMouseEvents();

		static void CallMouseEvents();

	protected:
		virtual bool mouseDown() = 0;
		virtual bool mouseUp() = 0;
		virtual bool mouseHovered() = 0;
		virtual bool mouseHovering() = 0;
		virtual bool mouseUnHovered() = 0;
		virtual bool mouseDragging() = 0;
		virtual bool mouseWheel() = 0;

		void callMouseEventHandler(const MouseEvent& e) const;
	};
}