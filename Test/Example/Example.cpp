#include "Aoba/Aoba.hpp"

class ToolBar final : public aoba::UIView {
private:
    std::function<void()> m_toggleColorModeHandler;
    std::function<void(const FilePath&)> m_folderOpenedHandler;
    std::function<void()> m_openParentDirHandler;

    aoba::UIToggleButton& m_uiToggleColorModeButton = aoba::Factory::Create<aoba::UIToggleButton>(this);
    aoba::UIButton& m_uiParentDirButton             = aoba::Factory::Create<aoba::UIButton>(this);
    aoba::UIButton& m_uiOpenDirectoryButton         = aoba::Factory::Create<aoba::UIButton>(this);

public:
    using UIView::UIView;

    void setToggleColorModeHandler(const std::function<void()>& handler) {
        m_toggleColorModeHandler = handler;
    }

    void setFolderOpendHandler(const std::function<void(const FilePath&)>& handler) {
        m_folderOpenedHandler = handler;
    }

    void setOpenParentDirHandler(const std::function<void()>& handler) {
        m_openParentDirHandler = handler;
    }

protected:
    void initialize() override {
        UIView::initialize();

        layout<aoba::AlignH>({aoba::LOConstant(m_uiToggleColorModeButton, 40_px),
                              aoba::LOConstant(m_uiParentDirButton, 40_px),
                              aoba::LOConstant(m_uiOpenDirectoryButton, 120_px)});

        m_uiToggleColorModeButton.setIcon(Texture(Icon(0xf186), 20_px));
        m_uiToggleColorModeButton.tooltipMessage = U"Toggle color mode";
        m_uiToggleColorModeButton.addEventListener<aoba::Event::Mouse::LeftDown>(
            [this] { m_toggleColorModeHandler(); });

        m_uiParentDirButton.setIcon(Texture(Icon(0xf062), 20_px));
        m_uiParentDirButton.addEventListener<aoba::Event::Mouse::LeftDown>([this] { m_openParentDirHandler(); });

        m_uiOpenDirectoryButton.setText(U"Open");
        m_uiOpenDirectoryButton.addEventListener<aoba::Event::Mouse::LeftDown>([this] {
            const auto result = Dialog::SelectFolder();
            if (result.has_value()) {
                m_folderOpenedHandler(result.value());
            }
        });
    }
};

class FileView final : public aoba::UIView {
private:
    FilePath m_path;
    std::function<void(const FilePath& path)> m_folderSelectedHandler;

    aoba::UIIcon& m_uiIcon        = aoba::Factory::Create<aoba::UIIcon>(this);
    aoba::UIText& m_uiFileName    = aoba::Factory::Create<aoba::UIText>(this);
    aoba::UIText& m_uiUpdatedDate = aoba::Factory::Create<aoba::UIText>(this);
    aoba::UIText& m_uiKind        = aoba::Factory::Create<aoba::UIText>(this);

public:
    using aoba::UIView::UIView;

    void setPath(const FilePath& path) {
        m_uiIcon.setIcon(FileSystem::IsDirectory(path) ? Icon(0xf07b) : Icon(0xf15b), 20_px);
        m_uiIcon.iconColor =
            FileSystem::IsDirectory(path) ? aoba::ColorTheme(Color(246, 218, 75)) : aoba::DynamicColor::Text;

        m_uiFileName.setText(FileSystem::FileName(path));

        m_uiKind.setText(FileSystem::IsDirectory(path) ? U"フォルダ" : U"ファイル");

        m_path = path;

        const auto time = FileSystem::WriteTime(path);
        if (time.has_value()) {
            m_uiUpdatedDate.setText(time.value().format());
        }
    }

    void setFolderSelectedHandler(const std::function<void(const FilePath& path)>& handler) {
        m_folderSelectedHandler = handler;
    }

protected:
    void initialize() override {
        UIView::initialize();

        layout<aoba::AlignH>({
            aoba::LOFunction(m_uiIcon, [this] { return layer().height(); }),
            aoba::LOFill(m_uiFileName),
            aoba::LOConstant(m_uiUpdatedDate, 220_px),
            aoba::LOConstant(m_uiKind, 80_px),
        });

        addEventListener<aoba::Event::Mouse::Hovered>(
            [this] { backgroundColor.highlight(aoba::DynamicColor::BackgroundSecondary); });
        addEventListener<aoba::Event::Mouse::UnHovered>(
            [this] { backgroundColor.lowlight(aoba::DynamicColor::Background); });
        addEventListener<aoba::Event::Mouse::LeftDown>([this] {
            if (FileSystem::IsDirectory(m_path)) {
                m_folderSelectedHandler(m_path);
            }
        });

        m_uiIcon.penetrateMouseEvent        = true;
        m_uiFileName.penetrateMouseEvent    = true;
        m_uiUpdatedDate.penetrateMouseEvent = true;
        m_uiKind.penetrateMouseEvent        = true;

        m_uiUpdatedDate.backgroundColor = aoba::DynamicColor::Background;
        m_uiUpdatedDate.setPadding(0, 0, 5_px, 5_px);

        m_uiKind.backgroundColor = aoba::DynamicColor::Background;
        m_uiKind.setPadding(0, 0, 5_px, 5_px);
    }
};

class ExplorerPage : public aoba::Page {
private:
    using Page::Page;

    FilePath m_path;

    ToolBar& m_uiToolbar              = aoba::Factory::Create<ToolBar>(view);
    aoba::UIVStackView& m_uiFilesView = aoba::Factory::Create<aoba::UIVStackView>(view);
    aoba::UIButton& m_uiMovePage      = aoba::Factory::Create<aoba::UIButton>(view);

protected:
    void onLoaded() override {
        view.layout<aoba::AlignV>(
            {aoba::LOConstant(m_uiToolbar, 30_px), aoba::LOFill(m_uiFilesView), aoba::LOConstant(m_uiMovePage, 40_px)});

        m_uiToolbar.backgroundColor = aoba::DynamicColor::BackgroundSecondary;
        m_uiToolbar.drawFrame       = true;
        m_uiToolbar.setToggleColorModeHandler([this] { aoba::Core::ToggleColorMode(); });
        m_uiToolbar.setFolderOpendHandler([this](const FilePath& path) { setup(path); });
        m_uiToolbar.setOpenParentDirHandler([this] {
            const FilePath path = FileSystem::ParentPath(m_path);
            setup(path);
        });

        m_uiFilesView.setLeadingDirection(aoba::LeadingDirection::Bottom);
        m_uiFilesView.setRowHeight(30_px);
        m_uiFilesView.setVerticalMargin(20_px);

        m_uiMovePage.setText(U"Move page");
        m_uiMovePage.addEventListener<aoba::Event::Mouse::LeftDown>([this] { aoba::Core::SwitchPage(U"start"); });
    }

private:
    void setup(const FilePath& dir) {
        m_path = dir;

        m_uiFilesView.release();

        const auto contents = FileSystem::DirectoryContents(dir, Recursive::No);
        for (const auto& path : contents) {
            auto& row = aoba::Factory::Create<FileView>(m_uiFilesView);
            row.setPath(path);
            row.penetrateMouseEvent = true;
            row.setFolderSelectedHandler([this](FilePath path) { setup(path); });
        }
    }
};

class StartPage : public aoba::Page {
private:
    using Page::Page;

    aoba::UIText& m_uiTitle            = aoba::Factory::Create<aoba::UIText>(view);
    aoba::UIInputField& m_uiInputField = aoba::Factory::Create<aoba::UIInputField>(view);
    aoba::UIButton& m_uiButton         = aoba::Factory::Create<aoba::UIButton>(view);

protected:
    void onLoaded() override {
        view.layout<aoba::AlignV>(
            {aoba::LOFill(m_uiTitle), aoba::LOConstant(m_uiInputField, 100_px), aoba::LOConstant(m_uiButton, 100_px)});

        registerKeyShortcut(KeySpace, [] { Print << U"Page shortcut: space"; });

        m_uiTitle.setText(U"This is the example application of Aoba.");
        m_uiTitle.setFont(aoba::UnifiedFontStyle::Header);
        m_uiTitle.setDirection(aoba::TextDirection::Center);
        m_uiTitle.registerKeyShortcut(KeyEnter, [] { Print << U"Key enter"; });
        m_uiTitle.registerKeyShortcut(KeyDelete, ClearPrint);
        m_uiTitle.registerKeyShortcut(KeyControl + KeyS, [] { Print << U"Ctrl+S Save"; });
        m_uiTitle.registerKeyShortcut(KeyLeft | KeyRight, [] { Print << U"Key left or right"; });

        m_uiInputField.setText(U"This is the example application of Aoba.");

        m_uiButton.setText(U"Next");
        m_uiButton.addEventListener<aoba::Event::Mouse::LeftDown>([this] { aoba::Core::SwitchPage(U"explorer"); });
    }
};

void AobaMain() {
    Window::Resize(1280, 720);

    aoba::Core::AppendPage<StartPage>(U"start");
    aoba::Core::AppendPage<ExplorerPage>(U"explorer");

    aoba::Core::Start();
}
