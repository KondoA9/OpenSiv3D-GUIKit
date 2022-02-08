#include "Aoba/Core.hpp"

#include <thread>

#include "Aoba/Page.hpp"
#include "Aoba/UnifiedFont.hpp"
#include "PageManager.hpp"
#include "TaskRunner.hpp"
#include "WindowManager.hpp"
#include "src/ComponentStorage/ComponentStorage.hpp"

namespace s3d::aoba {
    Core::Core() {
        m_pageManager = new PageManager();
        m_taskRunner  = new TaskRunner();

        WindowManager::Initialize();

        UnifiedFont::Initialize();

        Window::SetTitle(U"Aoba Framework");

        Scene::SetResizeMode(ResizeMode::Actual);

        System::SetTerminationTriggers(UserAction::NoAction);

        Core::AddLicense();
    }

    Core::~Core() {
        delete m_pageManager;
        delete m_taskRunner;
    }

    Core& Core::Instance() {
        static Core instance;
        return instance;
    }

    bool Core::IsAsyncTaskAlive() {
        return Instance().m_taskRunner->isAsyncTaskAlive();
    }

    bool Core::IsTerminationPrevented() {
        return Instance().m_terminationPrevented;
    }

    bool Core::IsTimeoutAlive(size_t id) {
        for (auto& timeout : Instance().m_timeouts) {
            if (timeout.id() == id) {
                return timeout.isAlive();
            }
        }
        return false;
    }

    void Core::SwitchPage(const String& identifier) {
        Instance().m_pageManager->switchPage(identifier);
    }

    void Core::AppendIsolatedComponent(const UIComponent& component) {
        Instance().appendIsolatedComponent(ComponentStorage::Get(component.id()));
    }

    void Core::SetColorMode(ColorMode mode) {
        Instance().m_animateColor = true;
        ColorTheme::SetColorMode(mode);
    }

    void Core::ToggleColorMode() {
        Instance().SetColorMode(ColorTheme::CurrentColorMode() == ColorMode::Light ? ColorMode::Dark
                                                                                   : ColorMode::Light);
    }

    void Core::PreventTermination() {
        Instance().m_terminationPrevented = true;
    }

    void Core::ContinueTermination() {
        Instance().m_terminationPrevented = false;
    }

    void Core::PostAsyncTask(const std::function<void()>& task, const std::function<void()>& completion) {
        if (completion) {
            Instance().m_taskRunner->addAsyncTask(task, [completion] { PostSyncTask(completion); });
        } else {
            Instance().m_taskRunner->addAsyncTask(task);
        }
    }

    void Core::PostSyncTask(const std::function<void()>& task) {
        Instance().m_taskRunner->addSyncTask(task);
    }

    size_t Core::SetTimeout(const std::function<void()>& func, double ms, bool threading) {
        Instance().m_timeouts.push_back(Timeout(func, ms, threading));
        return Instance().m_timeouts[Instance().m_timeouts.size() - 1].id();
    }

    bool Core::StopTimeout(size_t id) {
        for (auto& timeout : Instance().m_timeouts) {
            if (timeout.id() == id) {
                return timeout.stop();
            }
        }
        return false;
    }

    bool Core::RestartTimeout(size_t id) {
        for (auto& timeout : Instance().m_timeouts) {
            if (timeout.id() == id) {
                return timeout.restart();
            }
        }
        return false;
    }

    void Core::AddLicense() {
        LicenseInfo licence;
        licence.title     = U"Aoba Framework";
        licence.copyright = U"Copyright (c) 2021-2022 Ekyu Kondo";
        licence.text =
            UR"(Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.)";

        LicenseManager::AddLicense(licence);
    }

    Page& Core::getPage(const String& identifier) const noexcept {
        return m_pageManager->getPage(identifier);
    }

    bool Core::animateColor() {
        static double t = 0.0;
        t += 5.0 * Scene::DeltaTime();

        if (t > 1.0) {
            ColorTheme::Animate(ColorTheme::CurrentColorMode() == ColorMode::Light ? 0.0 : 1.0);
            t = 0.0;
            return false;
        }

        ColorTheme::Animate(ColorTheme::CurrentColorMode() == ColorMode::Light ? 1 - t : t);
        return true;
    }

    void Core::appendIsolatedComponent(const std::shared_ptr<UIComponent>& component) {
        m_pageManager->appendIsolatedComponent(component);
    }

    void Core::appendPage(const std::shared_ptr<Page>& page) {
        m_pageManager->appendPage(page);
    }
}
