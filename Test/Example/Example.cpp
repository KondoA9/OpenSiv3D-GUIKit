#include <GUIKit/GUIKit.hpp>

class ToolBar final : public gui::UIView {
private:
	std::function<void()> m_toggleColorModeHandler;
	std::function<void(const FilePath&)> m_folderOpenedHandler;
	std::function<void()> m_openParentDirHandler;

	gui::UIToggleButton& m_uiToggleColorModeButton = gui::GUIFactory::Create<gui::UIToggleButton>(this);
	gui::UIButton& m_uiParentDirButton = gui::GUIFactory::Create<gui::UIButton>(this);
	gui::UIButton& m_uiOpenDirectoryButton = gui::GUIFactory::Create<gui::UIButton>(this);

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

		m_uiOpenDirectoryButton.setText(U"Open");
		m_uiParentDirButton.setIcon(Texture(Icon(0xf062), 20_px));
		m_uiToggleColorModeButton.setIcon(Texture(Icon(0xf186), 20_px));

		m_uiToggleColorModeButton.setConstraint(gui::LayerDirection::Top, *this, gui::LayerDirection::Top);
		m_uiToggleColorModeButton.setConstraint(gui::LayerDirection::Bottom, *this, gui::LayerDirection::Bottom);
		m_uiToggleColorModeButton.setConstraint(gui::LayerDirection::Left);
		m_uiToggleColorModeButton.setConstraint(gui::LayerDirection::Width, 40_px);
		m_uiToggleColorModeButton.addEventListener<gui::MouseEvent::LeftDown>([this] {
			m_toggleColorModeHandler();
			});

		m_uiParentDirButton.setConstraint(gui::LayerDirection::Top, *this, gui::LayerDirection::Top);
		m_uiParentDirButton.setConstraint(gui::LayerDirection::Bottom, *this, gui::LayerDirection::Bottom);
		m_uiParentDirButton.setConstraint(gui::LayerDirection::Left, m_uiToggleColorModeButton, gui::LayerDirection::Right);
		m_uiParentDirButton.setConstraint(gui::LayerDirection::Width, 40_px);
		m_uiParentDirButton.addEventListener<gui::MouseEvent::LeftDown>([this] {
			m_openParentDirHandler();
			});

		m_uiOpenDirectoryButton.setConstraint(gui::LayerDirection::Top, *this, gui::LayerDirection::Top);
		m_uiOpenDirectoryButton.setConstraint(gui::LayerDirection::Bottom, *this, gui::LayerDirection::Bottom);
		m_uiOpenDirectoryButton.setConstraint(gui::LayerDirection::Left, m_uiParentDirButton, gui::LayerDirection::Right);
		m_uiOpenDirectoryButton.setConstraint(gui::LayerDirection::Width, 120_px);
		m_uiOpenDirectoryButton.addEventListener<gui::MouseEvent::LeftDown>([this] {
			const auto result = Dialog::SelectFolder();
			if (result.has_value()) {
				m_folderOpenedHandler(result.value());
			}
			});
	}
};

class FileView final : public gui::UIView {
private:
	FilePath m_path;
	std::function<void(const FilePath& path)> m_folderSelectedHandler;

	gui::UIIcon& m_uiIcon = gui::GUIFactory::Create<gui::UIIcon>(this);
	gui::UIText& m_uiFileName = gui::GUIFactory::Create<gui::UIText>(this);
	gui::UIText& m_uiUpdatedDate = gui::GUIFactory::Create<gui::UIText>(this);
	gui::UIText& m_uiKind = gui::GUIFactory::Create<gui::UIText>(this);

public:
	FileView()
		:UIView()
	{
		m_uiUpdatedDate.backgroundColor = gui::DynamicColor::Background;
		m_uiKind.backgroundColor = gui::DynamicColor::Background;
	}

	void setPath(const FilePath& path) {
		m_uiIcon.setIcon(FileSystem::IsDirectory(path) ? Icon(0xf07b) : Icon(0xf15b), 20_px);
		m_uiIcon.iconColor = FileSystem::IsDirectory(path) ? gui::ColorTheme(Color(246, 218, 75)) : gui::DynamicColor::Text;

		m_uiFileName.setText(FileSystem::FileName(path));

		m_uiKind.setText(FileSystem::IsDirectory(path) ? U"Folder" : U"File");

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

		addEventListener<gui::MouseEvent::Hovered>([this] {
			backgroundColor.highlight(gui::DynamicColor::BackgroundSecondary);
			});
		addEventListener<gui::MouseEvent::UnHovered>([this] {
			backgroundColor.lowlight(gui::DynamicColor::Background);
			});
		addEventListener<gui::MouseEvent::LeftDown>([this] {
			if (FileSystem::IsDirectory(m_path)) {
				m_folderSelectedHandler(m_path);
			}
			});

		m_uiIcon.penetrateMouseEvent = true;
		m_uiIcon.setConstraint(gui::LayerDirection::Top, *this, gui::LayerDirection::Top);
		m_uiIcon.setConstraint(gui::LayerDirection::Bottom, *this, gui::LayerDirection::Bottom);
		m_uiIcon.setConstraint(gui::LayerDirection::Left, *this, gui::LayerDirection::Left);
		m_uiIcon.setConstraint(gui::LayerDirection::Width, *this, gui::LayerDirection::Height);

		m_uiFileName.penetrateMouseEvent = true;
		m_uiFileName.setConstraint(gui::LayerDirection::Top, *this, gui::LayerDirection::Top);
		m_uiFileName.setConstraint(gui::LayerDirection::Bottom, *this, gui::LayerDirection::Bottom);
		m_uiFileName.setConstraint(gui::LayerDirection::Left, m_uiIcon, gui::LayerDirection::Right);
		m_uiFileName.setConstraint(gui::LayerDirection::Right, m_uiUpdatedDate, gui::LayerDirection::Left);

		m_uiUpdatedDate.penetrateMouseEvent = true;
		m_uiUpdatedDate.setPadding(0, 0, 5_px, 5_px);
		m_uiUpdatedDate.setConstraint(gui::LayerDirection::Top, *this, gui::LayerDirection::Top);
		m_uiUpdatedDate.setConstraint(gui::LayerDirection::Bottom, *this, gui::LayerDirection::Bottom);
		m_uiUpdatedDate.setConstraint(gui::LayerDirection::Width, 220_px);
		m_uiUpdatedDate.setConstraint(gui::LayerDirection::Right, m_uiKind, gui::LayerDirection::Left);

		m_uiKind.penetrateMouseEvent = true;
		m_uiKind.setPadding(0, 0, 5_px, 5_px);
		m_uiKind.setConstraint(gui::LayerDirection::Top, *this, gui::LayerDirection::Top);
		m_uiKind.setConstraint(gui::LayerDirection::Bottom, *this, gui::LayerDirection::Bottom);
		m_uiKind.setConstraint(gui::LayerDirection::Width, 80_px);
		m_uiKind.setConstraint(gui::LayerDirection::Right, *this, gui::LayerDirection::Right);
	}
};

class ExplorerPage : public gui::Page {
private:
	using Page::Page;

	FilePath m_path;

	ToolBar& m_uiToolbar = gui::GUIFactory::Create<ToolBar>(view);
	gui::UIVStackView& m_uiFilesView = gui::GUIFactory::Create<gui::UIVStackView>(view);
	gui::UIButton& m_uiMovePage = gui::GUIFactory::Create<gui::UIButton>(view);

protected:
	void onLoaded() override {
		m_uiToolbar.backgroundColor = gui::DynamicColor::BackgroundSecondary;
		m_uiMovePage.setText(U"Move page");

		m_uiToolbar.drawFrame = true;
		m_uiToolbar.setConstraint(gui::LayerDirection::Top, view, gui::LayerDirection::Top);
		m_uiToolbar.setConstraint(gui::LayerDirection::Height, 30_px);
		m_uiToolbar.setConstraint(gui::LayerDirection::Left);
		m_uiToolbar.setConstraint(gui::LayerDirection::Right, view, gui::LayerDirection::Right);
		m_uiToolbar.setToggleColorModeHandler([this] {
			gui::GUIKit::ToggleColorMode();
			});
		m_uiToolbar.setFolderOpendHandler([this](const FilePath& path) {
			setup(path);
			});
		m_uiToolbar.setOpenParentDirHandler([this] {
			const FilePath path = FileSystem::ParentPath(m_path);
			setup(path);
			});

		m_uiFilesView.setLeadingDirection(gui::LeadingDirection::Bottom);
		m_uiFilesView.setRowHeight(30_px);
		m_uiFilesView.setConstraint(gui::LayerDirection::Top, m_uiToolbar, gui::LayerDirection::Bottom);
		m_uiFilesView.setConstraint(gui::LayerDirection::Bottom, m_uiMovePage, gui::LayerDirection::Top);
		m_uiFilesView.setConstraint(gui::LayerDirection::Left, view, gui::LayerDirection::Left);
		m_uiFilesView.setConstraint(gui::LayerDirection::Right, view, gui::LayerDirection::Right);

		m_uiMovePage.setConstraint(gui::LayerDirection::Height, 40_px);
		m_uiMovePage.setConstraint(gui::LayerDirection::Bottom, view, gui::LayerDirection::Bottom);
		m_uiMovePage.setConstraint(gui::LayerDirection::Left, view, gui::LayerDirection::Left);
		m_uiMovePage.setConstraint(gui::LayerDirection::Right, view, gui::LayerDirection::Right);
		m_uiMovePage.addEventListener<gui::MouseEvent::LeftDown>([this] {
			gui::GUIKit::SwitchPage(U"start");
			});
	}

private:
	void setup(const FilePath& dir) {
		m_path = dir;

		m_uiFilesView.release();

		const auto contents = FileSystem::DirectoryContents(dir, Recursive::No);
		for (const auto& path : contents) {
			auto& row = gui::GUIFactory::Create<FileView>(m_uiFilesView);
			row.setPath(path);
			row.penetrateMouseEvent = true;
			row.setFolderSelectedHandler([this](FilePath path) {
				setup(path);
				});
		}
	}
};

class StartPage : public gui::Page {
private:
	using Page::Page;

	gui::UIText& m_uiTitle = gui::GUIFactory::Create<gui::UIText>(view);
	gui::UIButton& m_uiButton = gui::GUIFactory::Create<gui::UIButton>(view);
	gui::UICheckBox& m_uiCheckBox = gui::GUIFactory::Create<gui::UICheckBox>(view);

protected:
	void onLoaded() override {
		m_uiTitle.setText(U"This is the example application of OpenSiv3D GUIKit.");
		m_uiTitle.setFont(gui::UnifiedFontStyle::Header);
		m_uiTitle.setDirection(gui::TextDirection::Center);

		m_uiButton.setText(U"Next");

		m_uiTitle.setConstraint(gui::LayerDirection::Top);
		m_uiTitle.setConstraint(gui::LayerDirection::Height, view, gui::LayerDirection::Height, 0.0, 0.5);
		m_uiTitle.setConstraint(gui::LayerDirection::Left);
		m_uiTitle.setConstraint(gui::LayerDirection::Right, view, gui::LayerDirection::Right);

		m_uiCheckBox.setConstraint(gui::LayerDirection::Height, 100_px);
		m_uiCheckBox.setConstraint(gui::LayerDirection::Bottom, m_uiButton, gui::LayerDirection::Top);
		m_uiCheckBox.setConstraint(gui::LayerDirection::Left);
		m_uiCheckBox.setConstraint(gui::LayerDirection::Right, view, gui::LayerDirection::Right);

		m_uiButton.setConstraint(gui::LayerDirection::Height, 100_px);
		m_uiButton.setConstraint(gui::LayerDirection::Bottom, view, gui::LayerDirection::Bottom);
		m_uiButton.setConstraint(gui::LayerDirection::Left);
		m_uiButton.setConstraint(gui::LayerDirection::Right, view, gui::LayerDirection::Right);
		m_uiButton.addEventListener<gui::MouseEvent::LeftDown>([this] {
			gui::GUIKit::SwitchPage(U"explorer");
			});
	}
};

void GUIMain() {
	Window::Resize(1280, 720);

	gui::GUIKit::AppendPage<StartPage>(U"start");
	gui::GUIKit::AppendPage<ExplorerPage>(U"explorer");

	gui::GUIKit::Start();
}
