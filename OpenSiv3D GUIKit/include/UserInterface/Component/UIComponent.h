#pragma once

#include "../../ColorTheme/DynamicColor.h"
#include "../MouseEvent/MouseEvent.h"
#include "../Layer/Layer.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class UIComponent {
		struct CallableMouseEvent {
			MouseEvent mouseEvent;
			Array<MouseEventHandler> handlers;
		};

	public:
		ColorTheme backgroundColor, frameColor;
		double frameThickness = 1.0;
		bool fillInner = true, drawFrame = false;
		bool penetrateMouseEvent = false;

	protected:
		Layer m_layer;
		bool m_mouseOver = false, m_preMouseOver = false;
		bool m_mouseLeftDraggingEnable = false, m_mouseRightDraggingEnable = false;
		bool m_hidden = false, m_exist = true;

	private:
		static Array<CallableMouseEvent> m_CallableMouseEvents;

		Array<Layer*> m_dependentLayers;
		Array<MouseEventHandler> m_mouseEventHandlers;
		bool m_needToUpdateLayer = true;

	public:
		UIComponent(const ColorTheme& _backgroundColor = DynamicColor::BackgroundSecondary, const ColorTheme& _frameColor = DynamicColor::Separator) :
			backgroundColor(_backgroundColor),
			frameColor(_frameColor)
		{}

		virtual ~UIComponent() = default;

		virtual void updateLayer();

		virtual bool updateLayerIfNeeded();

		virtual void draw() {};

		virtual void updateMouseEvent();

		const Layer& layer() {
			return m_layer;
		}

		void addEventListener(MouseEventType e, const std::function<void(const MouseEvent& e)>& f) {
			m_mouseEventHandlers.push_back(MouseEventHandler(e, f));
		}

		void addEventListener(MouseEventType e, const std::function<void()>& f) {
			m_mouseEventHandlers.push_back(MouseEventHandler(e, [f](const MouseEvent&) {f(); }));
		}

		void requestToUpdateLayer() {
			m_needToUpdateLayer = true;
		}

		void setHidden(bool hidden) {
			m_hidden = hidden;
		}

		void setExist(bool exist) {
			m_exist = exist;
		}

		bool isHidden() const {
			return m_hidden;
		}

		bool isExist() const {
			return m_exist;
		}

		void setConstraint(LayerDirection direction, UIComponent& component, LayerDirection toDirection, double constant = 0.0, double multiplier = 1.0);

		void setConstraint(LayerDirection direction, double constant = 0.0, double multiplier = 1.0);

		void setConstraint(LayerDirection direction, const std::function<double()>& func, double constant = 0.0, double multiplier = 1.0);

		void removeConstraint(LayerDirection direction);

		static void _ResetMouseEvents();

		static void _CallMouseEvents();

	protected:
		bool drawable() const {
			return !m_hidden && m_exist
				&& m_layer.top.value <= Window::ClientHeight() && m_layer.bottom.value >= 0
				&& m_layer.left.value <= Window::ClientWidth() && m_layer.right.value >= 0;
		}

		virtual bool mouseLeftDown() = 0;
		virtual bool mouseLeftUp() = 0;
		virtual bool mouseLeftDragging() = 0;
		virtual bool mouseRightDown() = 0;
		virtual bool mouseRightUp() = 0;
		virtual bool mouseRightDragging() = 0;
		virtual bool mouseHovered() = 0;
		virtual bool mouseHovering() = 0;
		virtual bool mouseUnHovered() = 0;
		virtual bool mouseWheel() = 0;

		void callMouseEventHandler(const MouseEvent& e) const;
	};
}