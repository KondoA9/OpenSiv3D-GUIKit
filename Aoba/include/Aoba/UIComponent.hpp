#pragma once

#include <Siv3D.hpp>

#include "DynamicColor.hpp"
#include "InputEventHandler.hpp"
#include "Layer.hpp"
#include "MouseEvent.hpp"

namespace s3d::aoba {
	class Core;
	class UIView;
	class Factory;
	class PageManager;

	AobaCreateInputEvent(UnFocused);
	AobaCreateInputEvent(Focused);

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

		friend Core;
		friend Factory;
		friend UIView;
		friend PageManager;

	public:
		ColorTheme backgroundColor, frameColor;
		double frameThickness = 1.0;
		bool fillInner = true, drawFrame = false;
		bool penetrateMouseEvent = false;
		bool hidden = false, exist = true, controllable = true;

	private:
		static Array<CallableInputEvent> m_CallableInputEvents;
		static std::shared_ptr<UIComponent> m_FocusedComponent, m_PreviousFocusedComponent;

		const size_t m_id;
		bool m_valid = false;

		Layer m_layer;
		Array<Layer*> m_dependentLayers;
		Rect m_drawableRegion = Rect();
		bool m_needToUpdateLayer = true;
		bool m_initialized = false;

		// Mouse event
		MouseCondition m_mouseCondition;
		double m_clickIntervalTimer = 0.0;// If mouse released within 0.5s, mouseDown event will be called
		bool m_mouseDownEnable = false, m_mouseDownRaw = false;
		bool m_mouseDragging = false;
		Vec2 m_clickedPos = Vec2();
		Array<InputEventHandler> m_inputEventHandlers;

	public:
		UIComponent() noexcept;

		UIComponent(const UIComponent&) = delete;

		UIComponent(UIComponent&&) = default;

		virtual ~UIComponent();

		virtual void release() {}

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
			return m_FocusedComponent && m_FocusedComponent->id() == m_id;
		}

		bool updatable() const {
			return exist;
		}

		bool layerUpdatable() const {
			return updatable();
		}

		bool drawable() const {
			const bool insideRegion =
				m_layer.top() <= static_cast<double>(m_drawableRegion.y) + static_cast<double>(m_drawableRegion.h)
				&& m_layer.left() <= static_cast<double>(m_drawableRegion.x) + static_cast<double>(m_drawableRegion.w)
				&& m_layer.bottom() >= m_drawableRegion.y
				&& m_layer.right() >= m_drawableRegion.x;

			return updatable() && !hidden && insideRegion;
		}

		bool eventUpdatable() const {
			return drawable() && controllable;
		}

		void requestToUpdateLayer() {
			m_needToUpdateLayer = true;
		}

		void focus();

		void unFocus();

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

		UIComponent& operator =(const UIComponent&) = delete;

		UIComponent& operator =(UIComponent&&) = default;

	protected:
		// Called once before layer updated.
		// If you need to call addEventlistener or appnendComponent to implement the default behavior, define this function.
		// Do not forget to call super::initialize().
		virtual void initialize();

		virtual void update() {};

		virtual void updateLayer(const Rect& scissor);

		virtual void updateMouseIntersection() = 0;

		virtual void updateInputEvents();

		virtual void draw() const = 0;

		const MouseCondition& mouseCondition() const {
			return m_mouseCondition;
		}

		// Do not call this function if the component is not UIView
		virtual void _destroy();

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
						auto& behindComponentEvents = m_CallableInputEvents[i];
						if (behindComponentEvents.mouseEvent.id == e.id
							&& behindComponentEvents.mouseEvent.callIfComponentInFront) {
							behindComponentEvents.mouseEvent = e;
							behindComponentEvents.handlers = handlers;
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
		static void UpdateFocusEvent();

		static void CallInputEvents();

		virtual bool updateLayerIfNeeded(const Rect& scissor);

		void validate() {
			m_valid = true;
		}
	};
}
