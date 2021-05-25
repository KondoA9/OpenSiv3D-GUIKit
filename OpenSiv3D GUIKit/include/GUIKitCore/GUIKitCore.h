#pragma once

#include "../Page/Page.h"
#include "../Window/WindowManager.h"
#include "../UserInterface/Font/UnifiedFont.h"

#include <Siv3D.hpp>

namespace s3d::gui {
	class GUIKit {
		String m_title;

		Array<std::shared_ptr<Page>> m_pages;

		std::shared_ptr<Page> m_drawingPage;

		// When the page changed, forwardPage will be appeared and backwardPage will be disappeared.
		std::shared_ptr<Page> m_forwardPage, m_backwardPage;

		bool m_animateColor = false;
		bool m_uiChanging = false;

	public:
		GUIKit() {
			initialize();
		}

		~GUIKit() {}

		void start() {
			if (m_pages.size() > 0) {
				m_forwardPage = m_pages[0];
				run();
			}
			else {
				Logger << U"Error(GUIKitCore): No pages are registered.";
			}
		}

		template<class T>
		void appendPage(T page) {
			page.guikit = this;
			m_pages.push_back(std::make_shared<T>(page));
		}

		void setTitle(const String& title) {
			m_title = title;
			Window::SetTitle(title);
		}

		void switchPage(const String& identifier) {
			if (const auto& ui = getUserInterface<Page>(identifier); !m_uiChanging && ui) {
				m_forwardPage = ui;
				m_backwardPage = m_drawingPage;
			}
			else {
				Logger << U"Error(GUIKitCore): Switched current ui to the ui identified as {}, but the ui does not exist."_fmt(identifier);
			}
		}

		template<class T>
		std::shared_ptr<T> getUserInterface(const String& identifier) {
			for (const auto& page : m_pages) {
				if (page->identifier == identifier) {
					return std::dynamic_pointer_cast<T>(page);
				}
			}

			Logger << U"Error(GUIKitCore): UserInterface identified as {} does not exist."_fmt(identifier);
			return nullptr;
		}

		void setColorMode(ColorMode mode) {
			m_animateColor = true;
			ColorTheme::setColorMode(mode);
		}

		void toggleColorMode() {
			setColorMode(ColorTheme::colorMode() == ColorMode::Light ? ColorMode::Dark : ColorMode::Light);
		}

	private:
		void initialize() {
			WindowManager::initialize();
			UnifiedFont::Initialize();

			Scene::SetScaleMode(ScaleMode::ResizeFill);
		}

		void run() {
			while (System::Update()) {
				update();
			}
		}

		void update() {
			WindowManager::update();

			// Load start page
			static bool startup = true;
			if (startup && m_forwardPage) {
				static bool appeared = false;
				if (!appeared) {
					m_forwardPage->onLoaded();
					m_forwardPage->loaded = true;
					m_forwardPage->onBeforeAppeared();
					m_drawingPage = m_forwardPage;
					appeared = true;
				}
				else {
					m_forwardPage->onAfrerAppeared();
					m_forwardPage = nullptr;
					startup = false;
				}
			}

			// Page changing
			static bool synchronized = false;
			static bool forwardUILoaded = false;
			static double pageColorMultiplier = 1.0;
			if (m_forwardPage && m_backwardPage) {
				if (!synchronized) {
					if (!forwardUILoaded) {
						if (!m_forwardPage->loaded) {
							m_forwardPage->onLoaded();
							m_forwardPage->loaded = true;
						}
						m_uiChanging = true;
						forwardUILoaded = true;
					}
					else {
						// Sync forwardPage.onAppeared & backwardPage.onDisappeared
						m_forwardPage->onBeforeAppeared();
						m_forwardPage->view.updateShape();
						m_backwardPage->onBeforeDisappeared();
						synchronized = true;
					}
				}
			}

			// Color theme
			if (m_animateColor) {
				animateColor();
			}

			// Page changed
			static bool uiChanged = false;
			if (uiChanged) {
				m_drawingPage = m_forwardPage;
				m_forwardPage->onAfrerAppeared();
				m_backwardPage->onAfrerDisappeared();
				m_forwardPage = nullptr;
				m_backwardPage = nullptr;
				synchronized = false;
				forwardUILoaded = false;
				uiChanged = false;
			}

			// Drawing pages while page changing
			if (m_uiChanging) {
				pageColorMultiplier -= 5.0 * Scene::DeltaTime();
				if (pageColorMultiplier < 0.0) {
					Graphics2D::Internal::SetColorMul(ColorF(1.0, 1.0, 1.0, 1.0));
					m_forwardPage->view.draw();
					uiChanged = true;
					m_uiChanging = false;
					pageColorMultiplier = 1.0;
				}
				else {
					Graphics2D::Internal::SetColorMul(ColorF(1.0, 1.0, 1.0, 1.0 - pageColorMultiplier));
					m_forwardPage->view.draw();
					Graphics2D::Internal::SetColorMul(ColorF(1.0, 1.0, 1.0, pageColorMultiplier));
					m_backwardPage->view.draw();
				}
			}
			// Draw the page
			else {
				m_drawingPage->view.control();

				if (WindowManager::didResized()) {
					m_drawingPage->onWindowResized();
					m_drawingPage->view.updateShape();
				}
				else {
					m_drawingPage->view.updateShapeIfNeeded();
				}

				m_drawingPage->view.draw();
			}

			Graphics::SkipClearScreen();
		}

		void animateColor() {
			static double t = 0.0;
			if (ColorTheme::colorMode() == ColorMode::Dark) {
				t += 5.0 * Scene::DeltaTime();
				if (t > 1.0) {
					t = 1.0;
					m_animateColor = false;
				}
			}
			else {
				t -= 5.0 * Scene::DeltaTime();
				if (t < 0.0) {
					t = 0.0;
					m_animateColor = false;
				}
			}
			ColorTheme::animate(t);
		}
	};
}