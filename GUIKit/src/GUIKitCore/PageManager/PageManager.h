#include <Siv3D.hpp>

namespace s3d::gui {
	class Page;

	enum class PageTransition {
		StartUp,
		Stable,
		StartChanging,
		Changing,
		JustChanged,
		Termination
	};

	class PageManager final {
	private:
		PageTransition m_pageTransition = PageTransition::StartUp;

		Array<std::shared_ptr<Page>> m_pages;
		std::shared_ptr<Page> m_drawingPage, m_forwardPage, m_backwardPage;

		double m_pageTransitionRate = 1.0;

		Rect m_windowScissorRect;

	public:
		PageManager();

		PageManager(const PageManager&) = delete;

		PageManager(PageManager&&) = default;

		PageManager& operator =(const PageManager&) = delete;

		PageManager& operator =(PageManager&&) = delete;

		Page& getPage(const String& identifier) const noexcept;

		bool initialize();

		void update();

		void draw();

		void appendPage(const std::shared_ptr<Page>& page);

		void switchPage(const String& identifier);

		void setPageTransition(PageTransition transition) {
			m_pageTransition = transition;
		}

	private:
		std::shared_ptr<Page> getPagePtr(const String& identifier) const;

		// Return true until the start up page appeared.
		bool updateOnStartUp();

		// Return true until the page changed.
		bool updateOnPageChanging();

		void updateOnStable();

		void preparePageChanging();

		void finalizePageChanging();

		void updateOnTermination();

		void updateInputEvents();

		void updateLayers();
	};
}
