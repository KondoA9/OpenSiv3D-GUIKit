#include "Aoba/Page.hpp"

#include "Aoba/Factory.hpp"
#include "Aoba/UIView.hpp"
#include "src/KeyShortcut/KeyShortcut.hpp"

namespace s3d::aoba {
    Page::Page(const String& identifier) : view(Factory::_CreatePageView()), m_identifier(identifier) {
        acceptDragDrop(false, false);
        view.backgroundColor = DynamicColor::Background;

        view.setConstraint(LayerDirection::Top);
        view.setConstraint(LayerDirection::Bottom, [] { return Scene::Size().y; });
        view.setConstraint(LayerDirection::Left);
        view.setConstraint(LayerDirection::Right, [] { return Scene::Size().x; });
    }

    void Page::acceptDragDrop(bool acceptFiles, bool acceptTexts) {
        m_acceptDragDropFiles = acceptFiles;
        m_acceptDragDropTexts = acceptTexts;
    }

    Page::~Page() {}

    void Page::onLoaded() {}

    void Page::onBeforeAppeared() {}

    void Page::onAfterAppeared() {}

    void Page::onBeforeDisappeared() {}

    void Page::onAfterDisappeared() {}

    void Page::onWindowResized() {}

    void Page::onBeforeAppTerminated() {}

    void Page::onAppTerminated() {}

    void Page::onDragDrop(const Array<DroppedFilePath>&, const Array<DroppedText>&) {}

    void Page::registerKeyShortcut(const Input& input, const std::function<void()>& callback) {
        m_keyShortcuts.emplace_back(std::make_unique<KeyShortcut<Input>>(input, callback));
    }

    void Page::registerKeyShortcut(const InputCombination& input, const std::function<void()>& callback) {
        m_keyShortcuts.emplace_back(std::make_unique<KeyShortcut<InputCombination>>(input, callback));
    }

    void Page::registerKeyShortcut(const InputGroup& input, const std::function<void()>& callback) {
        m_keyShortcuts.emplace_back(std::make_unique<KeyShortcut<InputGroup>>(input, callback));
    }

    void Page::update() {
        for (auto& shortcut : m_keyShortcuts) {
            if (shortcut->keyDown()) {
                shortcut->call();
            }
        }
    }
}
