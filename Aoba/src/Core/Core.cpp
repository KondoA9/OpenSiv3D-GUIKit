#include "Aoba/Core.hpp"

#include <thread>

#include "Aoba/Factory.hpp"
#include "Aoba/Page.hpp"
#include "Aoba/UnifiedFont.hpp"
#include "src/ComponentStorage/ComponentStorage.hpp"
#include "src/Core/PageManager.hpp"
#include "src/Core/TaskRunner.hpp"
#include "src/Core/WindowManager.hpp"

namespace s3d::aoba {
    Core::Core() {
        m_mainThreadId = std::this_thread::get_id();

        m_pageManager = std::make_unique<PageManager>();
        m_taskRunner  = std::make_unique<TaskRunner>();

        WindowManager::Initialize();

        UnifiedFont::Initialize();

        Window::SetTitle(U"Aoba Framework");

        Scene::SetResizeMode(ResizeMode::Actual);

        System::SetTerminationTriggers(UserAction::NoAction);

        Core::AddLicense();
    }

    Core& Core::Instance() {
        static Core instance;
        return instance;
    }

    bool Core::IsMainThread() noexcept {
        return Instance().m_mainThreadId == std::this_thread::get_id();
    }

    bool Core::IsAsyncTaskAlive() noexcept {
        return Instance().m_taskRunner->isAsyncTaskAlive();
    }

    bool Core::IsTerminationPrevented() noexcept {
        return Instance().m_terminationPrevented;
    }

    bool Core::IsTimeoutAlive(size_t id) noexcept {
        return Instance().m_taskRunner->isTimeoutTaskAlive(id);
    }

    void Core::SwitchPage(StringView identifier) {
        Instance().m_pageManager->switchPage(identifier);
    }

    void Core::PreventTermination() noexcept {
        Instance().m_terminationPrevented = true;
    }

    void Core::ContinueTermination() noexcept {
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

    size_t Core::SetTimeout(const std::function<void()>& func, uint64 ms, bool threading) {
        return Instance().m_taskRunner->addTimeoutTask(func, ms, threading);
    }

    bool Core::StopTimeout(size_t id) noexcept {
        return Instance().m_taskRunner->stopTimeoutTask(id);
    }

    bool Core::RestartTimeout(size_t id) noexcept {
        return Instance().m_taskRunner->restartTimeoutTask(id);
    }

    void Core::NextFrame(const std::function<void()>& func) {
        if (func != nullptr) {
            Instance().m_nextFrameFunctions.emplace_back(func);
        }
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

    Page& Core::getPage(StringView identifier) const noexcept {
        return m_pageManager->getPage(identifier);
    }

    void Core::appendPage(const std::shared_ptr<Page>& page) {
        m_pageManager->appendPage(page);
    }

    void Core::callNextFrameFunctions() {
        // This implementation supports recursive calling of NextFrame()
        const auto count = m_nextFrameFunctions.size();
        if (count > 0) {
            for (size_t i = 0; i < count; i++) {
                m_nextFrameFunctions[i]();
            }
            m_nextFrameFunctions.erase(m_nextFrameFunctions.begin(), m_nextFrameFunctions.begin() + count);
        }
    }
}
