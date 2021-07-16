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

		struct MouseClickCondition {
			bool down = false, up = false, press = false;

			constexpr const MouseClickCondition& operator &=(bool condition) noexcept {
				down &= condition;
				up &= condition;
				press &= condition;
				return *this;
			}
		};

		struct MouseCondition {
			MouseClickCondition left, right;
			bool hover = false, preHover = false;

			constexpr const MouseCondition& operator &=(bool condition) noexcept {
				left &= condition;
				right &= condition;
				hover &= condition;
				return *this;
			}
		};

		friend GUIKit;
		friend UIView;

	public:
		ColorTheme backgroundColor, frameColor;
		double frameThickness = 1.0;
		bool fillInner = true, drawFrame = false;
		bool penetrateMouseEvent = false;
		bool hidden = false, exist = true, controllable = true;

	private:
		static Array<CallableInputEvent> m_CallableInputEvents;
		static UIComponent* m_FocusedComponent;

		const size_t m_id;
		Layer m_layer;
		Array<Layer*> m_dependentLayers;
		Rect m_drawableRegion;
		bool m_needToUpdateLayer = true;
		bool m_initialized = false;

		// Mouse event
		MouseCondition m_mouseCondition;
		double m_clickIntervalTimer = 0.0;// If mouse released within 0.5s, mouseDown event will be called
		bool m_mouseDownEnable = false, m_mouseDownRaw = false;
		bool m_mouseDragging = false;
		Vec2 m_clickedPos;
		Array<InputEventHandler> m_inputEventHandlers;

	public:
		UIComponent(const ColorTheme& _backgroundColor = DynamicColor::BackgroundSecondary, const ColorTheme& _frameColor = DynamicColor::Separator) noexcept;

		virtual ~UIComponent();

		void setConstraint(LayerDirection direction, UIComponent& component, LayerDirection toDirection, double constant = 0.0, double multiplier = 1.0);

		void setConstraint(LayerDirection direction, double constant = 0.0, double multiplier = 1.0);

		void setConstraint(LayerDirection direction, const std::function<double()>& func, double constant = 0.0, double multiplier = 1.0);

		void removeConstraint(LayerDirection direction);

		void removeAllConstraints();

		const Layer& layer() const {
			return m_layer;
		}

		size_t id() const {
			return m_id;
		}

		bool isFocused() const {
			return m_FocusedComponent == this;
		}

		bool drawable() const {
			return !hidden && exist
				&& m_layer.top <= m_drawableRegion.y + m_drawableRegion.h && m_layer.bottom >= m_drawableRegion.y
				&& m_layer.left <= m_drawableRegion.x + m_drawableRegion.w && m_layer.right >= m_drawableRegion.x;
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

		void unFocus() {
			m_FocusedComponent = nullptr;
		}

		template<class T>
		void addEventListener(const std::function<void(const T&)>& f, bool primary = false) {
			const auto handler = InputEventHandler::Create<T>(([f](InputEvent e) { f(*static_cast<T*>(&e)); }));

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
		virtual void initialize() {}

		virtual void release() {}

		virtual void updateLayer(const Rect& scissor);

		virtual void draw() = 0;

		virtual void updateMouseIntersection() = 0;

		virtual void updateInputEvents();

		const MouseCondition& mouseCondition() const {
			return m_mouseCondition;
		}

		// Do not call this function if the component is not UIRect or UICircle
		void _updateMouseCondition(
			bool leftDown, bool leftUp, bool leftPress,
			bool rightDown, bool rightUp, bool rightPress,
			bool hover
		);

		template<class T>
		void registerInputEvent(const T& e) const {
			// Get handlers that are matched to called event type
			const auto handlers = m_inputEventHandlers.removed_if([e](const InputEventHandler& handler) {
				return handler.eventTypeId != e.id;
				});

			if (e.callIfComponentInFront && !e.component->penetrateMouseEvent) {
				if (m_CallableInputEvents && m_CallableInputEvents[m_CallableInputEvents.size() - 1].mouseEvent.component != e.component) {
					m_CallableInputEvents = m_CallableInputEvents.removed_if([](const CallableInputEvent& e) {
						return e.mouseEvent.callIfComponentInFront;
						});
				}
				m_CallableInputEvents.push_back({ .mouseEvent = e, .handlers = handlers });
			}
			else {
				// Append handlers if event stack is empty or the component penetrates a mouse event
				if (!m_CallableInputEvents || e.component->penetrateMouseEvent) {
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
		}

	private:
		static void ResetInputEvents();

		static void CallInputEvents();

		virtual bool updateLayerIfNeeded(const Rect& scissor);
	};
}
