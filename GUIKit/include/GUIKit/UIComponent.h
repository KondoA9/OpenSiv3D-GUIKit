#pragma once

#include "DynamicColor.h"
#include "MouseEvent.h"
#include "InputEventHandler.h"
#include "Layer.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class GUIKit;
	class UIView;

	class UIComponent {
		struct CallableInputEvent {
			InputEvent mouseEvent;
			Array<InputEventHandler> handlers;
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
		static Array<CallableInputEvent> m_CallableInputEvents;
		static UIComponent* m_FocusedComponent;

		Array<Layer*> m_dependentLayers;
		Array<InputEventHandler> m_inputEventHandlers;
		bool m_needToUpdateLayer = true;
		bool m_initialized = false;

		// Mouse event
		double m_clickIntervalTimer = 0.0;// If mouse released within 0.5s, mouseDown event will be called
		bool m_mouseDownEnable = false, m_mouseDownRaw = false;
		bool m_mouseDragging = false;
		Vec2 m_clickedPos;

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

		bool drawable() const {
			return !hidden && exist
				&& m_layer.top <= Window::ClientHeight() && m_layer.bottom >= 0
				&& m_layer.left <= Window::ClientWidth() && m_layer.right >= 0;
		}

		bool updatable() const {
			return exist && controllable && drawable();
		}

		void requestToUpdateLayer() {
			m_needToUpdateLayer = true;
		}

		void focus() {
			m_FocusedComponent = this;
		}

		template<class T>
		void addEventListener(const std::function<void(const T&)>& f, bool primary = false) {
			auto handler = InputEventHandler([f](InputEvent e) { f(*static_cast<T*>(&e)); });
			handler.setEvent<T>();
			if (primary) {
				m_inputEventHandlers.push_front(handler);
			}
			else {
				m_inputEventHandlers.push_back(handler);
			}
		}

		template<class T>
		void addEventListener(const std::function<void()>& f, bool primary = false) {
			addEventListener<T>([f](const InputEvent&) { f(); }, primary);
		}

	protected:
		// Called once before layer updated.
		// If you need to call addEventlistener or appnendComponent to implement the default behavior, define this function.
		// Do not forget to call super::initialize() unless you do not want to call it.
		virtual void initialize() {};

		virtual void updateLayer();

		virtual void draw() = 0;

		virtual void updateMouseIntersection() = 0;

		virtual void updateInputEvents();

		template<class T>
		void callInputEventHandler(const T& e) const {
			// Get handlers that are matched to called event type
			const auto handlers = m_inputEventHandlers.removed_if([e](const InputEventHandler& handler) {
				return handler.eventTypeId != e.id;
				});

			// Append handlers if event stack is empty or the component penetrates a mouse event
			if (m_CallableInputEvents.size() == 0 || e.component->penetrateMouseEvent) {
				m_CallableInputEvents.push_back({ .mouseEvent = e, .handlers = handlers });
			}
			else {
				for (size_t i : step(m_CallableInputEvents.size())) {
					if (m_CallableInputEvents[i].mouseEvent.id == e.id) {
						m_CallableInputEvents[i].mouseEvent = e;
						m_CallableInputEvents[i].handlers = handlers;
						break;
					}
					// Append handler if a event that is same type of the event does not exists 
					else if (i == m_CallableInputEvents.size() - 1) {
						m_CallableInputEvents.push_back({ .mouseEvent = e, .handlers = handlers });
					}
				}
			}
		}

	private:
		static void ResetInputEvents();

		static void CallInputEvents();

		virtual bool updateLayerIfNeeded();
	};
}