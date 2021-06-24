#pragma once

#include "DynamicColor.h"
#include "MouseEvent.h"
#include "MouseEventHandler.h"
#include "Layer.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class GUIKit;
	class UIView;

	class UIComponent {
		struct CallableMouseEvent {
			IMouseEvent mouseEvent;
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

		// Mouse event
		bool m_mouseLeftDown = false, m_mouseLeftUp = false, m_mouseLeftPress = false;
		bool m_mouseRightDown = false, m_mouseRightUp = false, m_mouseRightPress = false;
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

		void requestToUpdateLayer() {
			m_needToUpdateLayer = true;
		}

		void focus() {
			m_FocusedComponent = this;
		}

		template<class T>
		void addEventListener(const std::function<void(const T&)>& f) {
			auto handler = MouseEventHandler([f](IMouseEvent e) { f(*static_cast<T*>(&e)); });
			handler.setEvent<T>();
			m_mouseEventHandlers.push_back(handler);
		}

		template<class T>
		void addEventListener(const std::function<void()>& f) {
			auto handler = MouseEventHandler([f](const IMouseEvent&) { f(); });
			handler.setEvent<T>();
			m_mouseEventHandlers.push_back(handler);
		}

	protected:
		virtual void updateLayer();

		virtual void draw() = 0;

		virtual void updateMouseIntersection() = 0;

		virtual void updateMouseEvents();

		template<class T>
		void callMouseEventHandler(const T& e) const {
			// Get handlers that are matched to called event type
			const auto handlers = m_mouseEventHandlers.removed_if([e](const MouseEventHandler& handler) {
				return handler.eventTypeId != e.id;
				});

			// Append handlers if event stack is empty or the component penetrates a mouse event
			if (m_CallableMouseEvents.size() == 0 || e.component->penetrateMouseEvent) {
				m_CallableMouseEvents.push_back({ .mouseEvent = e, .handlers = handlers });
			}
			else {
				for (size_t i : step(m_CallableMouseEvents.size())) {
					if (m_CallableMouseEvents[i].mouseEvent.id == e.id) {
						m_CallableMouseEvents[i].mouseEvent = e;
						m_CallableMouseEvents[i].handlers = handlers;
						break;
					}
					// Append handler if a event that is same type of the event does not exists 
					else if (i == m_CallableMouseEvents.size() - 1) {
						m_CallableMouseEvents.push_back({ .mouseEvent = e, .handlers = handlers });
					}
				}
			}
		}

	private:
		static void ResetMouseEvents();

		static void CallMouseEvents();

		virtual bool updateLayerIfNeeded();

		bool mouseLeftDown();

		bool mouseLeftUp();

		bool mouseLeftDragging();

		bool mouseRightDown();

		bool mouseRightUp();

		bool mouseRightDragging();

		bool mouseHovered();

		bool mouseHovering();

		bool mouseUnHovered();

		bool mouseWheel();

		bool drawable() const {
			return !hidden && exist
				&& m_layer.top <= Window::ClientHeight() && m_layer.bottom >= 0
				&& m_layer.left <= Window::ClientWidth() && m_layer.right >= 0;
		}

		bool updatable() const {
			return exist && controllable && drawable();
		}
	};
}