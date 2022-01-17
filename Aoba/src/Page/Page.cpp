#include <Aoba/Page.hpp>

#include <Aoba/Factory.hpp>
#include <Aoba/UIView.hpp>

namespace s3d::aoba {
	Page::Page(const String& identifier) :
		view(Factory::CreateComponent<UIView>()),
		m_identifier(identifier)
	{
		acceptDragDrop(false);

		view.setConstraint(LayerDirection::Top);
		view.setConstraint(LayerDirection::Bottom, [] {
			return Scene::Size().y;
			});
		view.setConstraint(LayerDirection::Left);
		view.setConstraint(LayerDirection::Right, [] {
			return Scene::Size().x;
			});
	}

	void Page::acceptDragDrop(bool accept) {
		m_acceptDragDrop = accept;
		DragDrop::AcceptFilePaths(accept);
		DragDrop::AcceptText(accept);
	}

	void Page::onLoaded() {}

	void Page::onBeforeAppeared() {}

	void Page::onAfterAppeared() {}

	void Page::onBeforeDisappeared() {}

	void Page::onAfterDisappeared() {}

	void Page::onWindowResized() {}

	void Page::onBeforeAppTerminated() {}

	void Page::onAppTerminated() {}

	void Page::onDragDrop(const Array<DroppedFilePath>&, const Array<DroppedText>&) {}
}
