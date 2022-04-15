#include "Aoba/ColorTheme.hpp"
#include "Aoba/Core.hpp"
#include "src/AobaLog/AobaLog.hpp"
#include "src/Core/PageManager.hpp"
#include "src/Core/TaskRunner.hpp"
#include "src/Core/WindowManager.hpp"

void AobaMain();

void Main() {
    (void)s3d::aoba::Core::Instance();
    AobaMain();
}

namespace s3d::aoba {
    bool Core::Start() {
        if (Instance().m_pageManager->initialize()) {
            Instance().run();
            return true;
        } else {
            AobaLog::Log(AobaLog::Type::Error, U"Core", U"No pages are registered");
            return false;
        }
    }

    void Core::Terminate() {
        Instance().m_pageManager->terminate();
    }

    void Core::run() {
        while (System::Update()) {
            if (System::GetUserActions() == UserAction::CloseButtonClicked) {
                m_pageManager->terminate();
            }

            update();
        }
    }

    void Core::update() {
        WindowManager::Update();

        m_pageManager->update();

        if (m_animateColor) {
            m_animateColor = animateColor();
        }

        m_pageManager->draw();

        m_taskRunner->update();
    }
}
