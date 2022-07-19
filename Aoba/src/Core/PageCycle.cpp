#include "Aoba/Core.hpp"
#include "Aoba/Page.hpp"
#include "Aoba/UIView.hpp"
#include "PageManager.hpp"
#include "WindowManager.hpp"
#include "src/ComponentStorage/ComponentStorage.hpp"
#include "src/InputEvent/InputEventManager.hpp"

namespace s3d::aoba {
    void PageManager::update() {
        // Load nextPage
        if (m_nextPage && !m_nextPage->didLoaded()) {
            m_nextPage->onLoaded();
            m_nextPage->m_loaded = true;
        }

        if (WindowManager::DidResized()) {
            // Update scissor rect
            const auto size     = Scene::Size();
            m_windowScissorRect = Rect(0, 0, size.x, size.y);

            // Call window resize event
            if (m_nextPage) {
                m_nextPage->onWindowResized();
            }

            if (m_currentPage) {
                m_currentPage->onWindowResized();
            }

            if (m_previousPage) {
                m_previousPage->onWindowResized();
            }
        }

        // Update DragDrop
        if (m_nextPage) {
            const auto [acceptFiles, acceptTexts] = m_nextPage->isDragDropAccepted();
            DragDrop::AcceptFilePaths(acceptFiles);
            DragDrop::AcceptText(acceptTexts);
        }

        if (m_currentPage) {
            if (const auto [acceptFiles, acceptTexts] = m_currentPage->isDragDropAccepted();
                (acceptFiles || acceptTexts) && (DragDrop::HasNewFilePaths() || DragDrop::HasNewText())) {
                m_currentPage->onDragDrop(DragDrop::GetDroppedFilePaths(), DragDrop::GetDroppedText());
                DragDrop::Clear();
            }
        }

        if (m_currentPage) {
            m_currentPage->update();
        }

        updateComponents();

        updateLayers();

        // Page transition
        switch (m_pageTransition) {
        case PageTransition::StartUp:
            if (!updateOnStartUp()) {
                m_pageTransition = PageTransition::Stable;
            }
            break;

        case PageTransition::Stable:
            updateOnStable();
            break;

        case PageTransition::StartChanging:
            preparePageChanging();
            m_pageTransition = PageTransition::Changing;
            break;

        case PageTransition::Changing:
            if (!updateOnPageChanging()) {
                m_pageTransition = PageTransition::JustChanged;
            }
            break;

        case PageTransition::JustChanged:
            finalizePageChanging();
            m_pageTransition = PageTransition::Stable;
            break;

        case PageTransition::Termination:
            updateOnTermination();
            break;

        default:
            assert(false);
            break;
        }
    }

    void PageManager::draw() {
        switch (m_pageTransition) {
        case PageTransition::StartUp:
            m_nextPage->view.draw();
            break;

        case PageTransition::Stable:
            m_currentPage->view.draw();
            break;

        case PageTransition::StartChanging:
            m_previousPage->view.draw();
            break;

        case PageTransition::Changing:
            // Draw previous and next page
            Graphics2D::Internal::SetColorMul(Float4(1.0, 1.0, 1.0, 1.0 - m_pageTransitionRate));
            m_nextPage->view.draw();
            Graphics2D::Internal::SetColorMul(Float4(1.0, 1.0, 1.0, m_pageTransitionRate));
            m_previousPage->view.draw();
            break;

        case PageTransition::JustChanged:
            // Initialize ColorMultipiler
            Graphics2D::Internal::SetColorMul(Float4(1.0, 1.0, 1.0, 1.0));
            m_nextPage->view.draw();
            break;

        case PageTransition::Termination:
            m_currentPage->view.draw();
            break;

        default:
            assert(false);
            break;
        }

        // Draw isolated components
        for (auto& component : ComponentStorage::GetIsolatedComponents()) {
            if (component->drawable()) {
                component->draw();
            }
        }
    }

    bool PageManager::updateOnStartUp() {
        // Run once when launching the app
        if (static bool called = false; !called) {
            m_nextPage->onBeforeAppeared();
            called = true;
        }
        // Run at next frame
        else {
            m_nextPage->onAfterAppeared();
            m_currentPage = m_nextPage;
            m_nextPage.reset();
            return false;
        }

        return true;
    }

    bool PageManager::updateOnPageChanging() {
        m_pageTransitionRate -= 5.0 * Scene::DeltaTime();

        // The page changed
        if (m_pageTransitionRate < 0.0) {
            m_pageTransitionRate = 1.0;
            return false;
        }

        return true;
    }

    void PageManager::updateOnStable() {
        assert(m_currentPage);

        updateInputEvents();
    }

    void PageManager::updateOnTermination() {
        static bool once = true;

        if (once) {
            for (auto& page : m_pages) {
                page->onBeforeAppTerminated();
            }

            once = false;
        }

        if (!(aoba::Core::IsTerminationPrevented() || aoba::Core::IsAsyncTaskAlive())) {
            m_currentPage->onBeforeDisappeared();

            for (auto& page : m_pages) {
                page->onAppTerminated();
            }

            System::Exit();
        }
    }

    void PageManager::preparePageChanging() {
        m_previousPage->onBeforeDisappeared();
        m_nextPage->onBeforeAppeared();

        // Request to update layer of the next page
        m_nextPage->view.requestToUpdateLayer();
    }

    void PageManager::finalizePageChanging() {
        m_previousPage->onAfterDisappeared();
        m_nextPage->onAfterAppeared();

        m_currentPage = m_nextPage;

        m_nextPage.reset();
        m_previousPage.reset();
    }

    void PageManager::updateComponents() {
        if (m_nextPage) {
            m_nextPage->view.update();
        }

        if (m_currentPage) {
            m_currentPage->view.update();
        }

        if (m_previousPage) {
            m_previousPage->view.update();
        }

        for (auto& component : ComponentStorage::GetIsolatedComponents()) {
            component->update();
        }
    }

    void PageManager::updateLayers() {
        if (WindowManager::DidResized()) {
            if (m_nextPage) {
                m_nextPage->view.updateLayer(m_windowScissorRect);
                m_nextPage->view.updateLayerInvert(m_windowScissorRect);
            }

            if (m_currentPage) {
                m_currentPage->view.updateLayer(m_windowScissorRect);
                m_currentPage->view.updateLayerInvert(m_windowScissorRect);
            }

            if (m_previousPage) {
                m_previousPage->view.updateLayer(m_windowScissorRect);
                m_previousPage->view.updateLayerInvert(m_windowScissorRect);
            }

            for (auto& component : ComponentStorage::GetIsolatedComponents()) {
                component->updateLayer(m_windowScissorRect);
            }
        } else {
            if (m_nextPage) {
                m_nextPage->view.updateLayerIfNeeded(m_windowScissorRect);
            }

            if (m_currentPage) {
                m_currentPage->view.updateLayerIfNeeded(m_windowScissorRect);
            }

            if (m_previousPage) {
                m_previousPage->view.updateLayerIfNeeded(m_windowScissorRect);
            }

            for (auto& component : ComponentStorage::GetIsolatedComponents()) {
                component->updateLayerIfNeeded(m_windowScissorRect);
            }
        }
    }

    void PageManager::updateInputEvents() {
        if (m_currentPage->view.eventUpdatable()) {
            m_currentPage->view.updateMouseIntersection();
            m_currentPage->view.updateInputEvents();
        }

        for (auto& component : ComponentStorage::GetIsolatedComponents()) {
            if (component->eventUpdatable()) {
                component->updateMouseIntersection();
                component->updateInputEvents();
            }
        }

        InputEventManager::Call();
    }
}
