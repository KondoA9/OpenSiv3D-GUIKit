#pragma once

#include "DynamicColor.h"
#include "MouseEvent.h"
#include "Layer.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class GUIKit;
	class UIView;

	class UIComponent {
		struct CallableMouseEvent {
			MouseEvent mouseEvent;
			Array<MouseEventHandler> handlers;
		};

		friend GUIKit;
		friend UIView;

	public:
		ColorTheme backgroundColor, frameColor;
		double frameThickness = 1.0;
		bool fillInner = true, drawFrame = false;
		bool penetrateMouseEvent = false;
		bool hidden = false, exist = true, controllable = true;

	protected:
		Layer m_layer;
		bool m_mouseOver = false, m_preMouseOver = false;
		bool m_mouseLeftDraggingEnable = false, m_mouseRightDraggingEnable = false;

	private:
		static Array<CallableMouseEvent> m_CallableMouseEvents;
		static UIComponent* m_FocusedComponent;

		Array<Layer*> m_dependentLayers;
		Array<MouseEventHandler> m_mouseEventHandlers;
		bool m_needToUpdateLayer = true;

	public:
		UIComponent(const ColorTheme& _backgroundColor = DynamicColor::BackgroundSecondary, const ColorTheme& _frameColor = DynamicColor::Separator) :
			backgroundColor(_backgroundColor),
			frameColor(_frameColor)
		{}

		virtual ~UIComponent() = default;

		void setConstraint(LayerDirection direction, UIComponent & component, LayerDirection toDirection, double constant = 0.0, double multiplier = 1.0);

		void setConstraint(LayerDirection direction, double constant = 0.0, double multiplier = 1.0);

		void setConstraint(LayerDirection direction, const std::function<double()>&func, double constant = 0.0, double multiplier = 1.0);

		void removeConstraint(LayerDirection direction);

		void removeAllConstraints();

		const Layer& layer() const {
			return m_layer;
		}

		bool isFocused() const {
			return m_FocusedComponent == this;
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

		void focus() {
			m_FocusedComponent = this;
		}

	protected:
		virtual void updateLayer();

		virtual void draw() = 0;

		virtual void update() = 0;

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

	private:
		static void ResetMouseEvents();

		static void CallMouseEvents();

		virtual bool updateLayerIfNeeded();

		virtual void updateMouseEvent();

		bool updatable() const {
			return exist && controllable;
		}

		bool drawable() const {
			return !hidden && exist
				&& m_layer.top.value <= Window::ClientHeight() && m_layer.bottom.value >= 0
				&& m_layer.left.value <= Window::ClientWidth() && m_layer.right.value >= 0;
		}
	};
}