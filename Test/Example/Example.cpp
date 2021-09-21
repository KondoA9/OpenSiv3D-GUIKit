#include <GUIKit/GUIKit.hpp>

class ToolBar final : public gui::UIView {
	std::function<void()> m_toggleColorModeHandler;
	std::function<void(const FilePath&)> m_folderOpenedHandler;
	std::function<void()> m_openParentDirHandler;

	gui::UIToggleButton& ui_toggleColorModeButton = gui::GUIFactory::Create<gui::UIToggleButton>(this);
	gui::UIButton& ui_parentDirButton = gui::GUIFactory::Create<gui::UIButton>(this);
	gui::UIButton& ui_openDirectoryButton = gui::GUIFactory::Create<gui::UIButton>(this);

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

		ui_openDirectoryButton.setText(U"Open");
		ui_parentDirButton.setIcon(Texture(Icon(0xf062), 20_px));
		ui_toggleColorModeButton.setIcon(Texture(Icon(0xf186), 20_px));

		ui_toggleColorModeButton.setConstraint(gui::LayerDirection::Top, *this, gui::LayerDirection::Top);
		ui_toggleColorModeButton.setConstraint(gui::LayerDirection::Bottom, *this, gui::LayerDirection::Bottom);
		ui_toggleColorModeButton.setConstraint(gui::LayerDirection::Left);
		ui_toggleColorModeButton.setConstraint(gui::LayerDirection::Width, 40_px);
		ui_toggleColorModeButton.addEventListener<gui::MouseEvent::LeftDown>([this] {
			m_toggleColorModeHandler();
			});

		ui_parentDirButton.setConstraint(gui::LayerDirection::Top, *this, gui::LayerDirection::Top);
		ui_parentDirButton.setConstraint(gui::LayerDirection::Bottom, *this, gui::LayerDirection::Bottom);
		ui_parentDirButton.setConstraint(gui::LayerDirection::Left, ui_toggleColorModeButton, gui::LayerDirection::Right);
		ui_parentDirButton.setConstraint(gui::LayerDirection::Width, 40_px);
		ui_parentDirButton.addEventListener<gui::MouseEvent::LeftDown>([this] {
			m_openParentDirHandler();
			});

		ui_openDirectoryButton.setConstraint(gui::LayerDirection::Top, *this, gui::LayerDirection::Top);
		ui_openDirectoryButton.setConstraint(gui::LayerDirection::Bottom, *this, gui::LayerDirection::Bottom);
		ui_openDirectoryButton.setConstraint(gui::LayerDirection::Left, ui_parentDirButton, gui::LayerDirection::Right);
		ui_openDirectoryButton.setConstraint(gui::LayerDirection::Width, 120_px);
		ui_openDirectoryButton.addEventListener<gui::MouseEvent::LeftDown>([this] {
			const auto result = Dialog::SelectFolder();
			if (result.has_value()) {
				m_folderOpenedHandler(result.value());
			}
			});
	}
};

class FileView final : public gui::UIView {
	FilePath m_path;
	std::function<void(const FilePath& path)> m_folderSelectedHandler;

	gui::UIIcon& ui_icon = gui::GUIFactory::Create<gui::UIIcon>(this);
	gui::UIText& ui_fileName = gui::GUIFactory::Create<gui::UIText>(this);
	gui::UIText& ui_updatedDate = gui::GUIFactory::Create<gui::UIText>(this);
	gui::UIText& ui_kind = gui::GUIFactory::Create<gui::UIText>(this);

public:
	FileView()
		:UIView()
	{
		ui_updatedDate.backgroundColor = gui::DynamicColor::Background;
		ui_kind.backgroundColor = gui::DynamicColor::Background;
	}

	void setPath(const FilePath& path) {
		ui_icon.setIcon(FileSystem::IsDirectory(path) ? Icon(0xf07b) : Icon(0xf15b), 20_px);
		ui_icon.iconColor = FileSystem::IsDirectory(path) ? gui::ColorTheme(Color(246, 218, 75)) : gui::DynamicColor::Text;

		ui_fileName.setText(FileSystem::FileName(path));

		ui_kind.setText(FileSystem::IsDirectory(path) ? U"フォルダ" : U"ファイル");

		m_path = path;

		const auto time = FileSystem::WriteTime(path);
		if (time.has_value()) {
			ui_updatedDate.setText(time.value().format());
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

		ui_icon.penetrateMouseEvent = true;
		ui_icon.setConstraint(gui::LayerDirection::Top, *this, gui::LayerDirection::Top);
		ui_icon.setConstraint(gui::LayerDirection::Bottom, *this, gui::LayerDirection::Bottom);
		ui_icon.setConstraint(gui::LayerDirection::Left, *this, gui::LayerDirection::Left);
		ui_icon.setConstraint(gui::LayerDirection::Width, *this, gui::LayerDirection::Height);

		ui_fileName.penetrateMouseEvent = true;
		ui_fileName.setConstraint(gui::LayerDirection::Top, *this, gui::LayerDirection::Top);
		ui_fileName.setConstraint(gui::LayerDirection::Bottom, *this, gui::LayerDirection::Bottom);
		ui_fileName.setConstraint(gui::LayerDirection::Left, ui_icon, gui::LayerDirection::Right);
		ui_fileName.setConstraint(gui::LayerDirection::Right, ui_updatedDate, gui::LayerDirection::Left);

		ui_updatedDate.penetrateMouseEvent = true;
		ui_updatedDate.setPadding(0, 0, 5_px, 5_px);
		ui_updatedDate.setConstraint(gui::LayerDirection::Top, *this, gui::LayerDirection::Top);
		ui_updatedDate.setConstraint(gui::LayerDirection::Bottom, *this, gui::LayerDirection::Bottom);
		ui_updatedDate.setConstraint(gui::LayerDirection::Width, 220_px);
		ui_updatedDate.setConstraint(gui::LayerDirection::Right, ui_kind, gui::LayerDirection::Left);

		ui_kind.penetrateMouseEvent = true;
		ui_kind.setPadding(0, 0, 5_px, 5_px);
		ui_kind.setConstraint(gui::LayerDirection::Top, *this, gui::LayerDirection::Top);
		ui_kind.setConstraint(gui::LayerDirection::Bottom, *this, gui::LayerDirection::Bottom);
		ui_kind.setConstraint(gui::LayerDirection::Width, 80_px);
		ui_kind.setConstraint(gui::LayerDirection::Right, *this, gui::LayerDirection::Right);
	}
};

class ExplorerPage : public gui::Page {
	using Page::Page;

	FilePath m_path;

	ToolBar& ui_toolbar = gui::GUIFactory::Create<ToolBar>(view());
	gui::UIVStackView& ui_filesView = gui::GUIFactory::Create<gui::UIVStackView>(view());
	gui::UIButton& ui_movePage = gui::GUIFactory::Create<gui::UIButton>(view());

	void onLoaded() override {
		ui_toolbar.backgroundColor = gui::DynamicColor::BackgroundSecondary;
		ui_movePage.setText(U"Move page");

		ui_toolbar.drawFrame = true;
		ui_toolbar.setConstraint(gui::LayerDirection::Top, view(), gui::LayerDirection::Top);
		ui_toolbar.setConstraint(gui::LayerDirection::Height, 30_px);
		ui_toolbar.setConstraint(gui::LayerDirection::Left);
		ui_toolbar.setConstraint(gui::LayerDirection::Right, view(), gui::LayerDirection::Right);
		ui_toolbar.setToggleColorModeHandler([this] {
			gui::GUIKit::ToggleColorMode();
			});
		ui_toolbar.setFolderOpendHandler([this](const FilePath& path) {
			setup(path);
			});
		ui_toolbar.setOpenParentDirHandler([this] {
			const FilePath path = FileSystem::ParentPath(m_path);
			setup(path);
			});

		ui_filesView.setLeadingDirection(gui::LeadingDirection::Bottom);
		ui_filesView.setRowHeight(30_px);
		ui_filesView.setConstraint(gui::LayerDirection::Top, ui_toolbar, gui::LayerDirection::Bottom);
		ui_filesView.setConstraint(gui::LayerDirection::Bottom, ui_movePage, gui::LayerDirection::Top);
		ui_filesView.setConstraint(gui::LayerDirection::Left, view(), gui::LayerDirection::Left);
		ui_filesView.setConstraint(gui::LayerDirection::Right, view(), gui::LayerDirection::Right);

		ui_movePage.setConstraint(gui::LayerDirection::Height, 40_px);
		ui_movePage.setConstraint(gui::LayerDirection::Bottom, view(), gui::LayerDirection::Bottom);
		ui_movePage.setConstraint(gui::LayerDirection::Left, view(), gui::LayerDirection::Left);
		ui_movePage.setConstraint(gui::LayerDirection::Right, view(), gui::LayerDirection::Right);
		ui_movePage.addEventListener<gui::MouseEvent::LeftDown>([this] {
			gui::GUIKit::SwitchPage(U"start");
			});
	}

	void setup(const FilePath& dir) {
		m_path = dir;

		ui_filesView.release();

		const auto contents = FileSystem::DirectoryContents(dir, Recursive::No);
		for (const auto& path : contents) {
			auto& row = gui::GUIFactory::Create<FileView>(ui_filesView);
			row.setPath(path);
			row.penetrateMouseEvent = true;
			row.setFolderSelectedHandler([this](FilePath path) {
				setup(path);
				});
		}
	}
};

class StartPage : public gui::Page {
	using Page::Page;

	gui::UIText& ui_title = gui::GUIFactory::Create<gui::UIText>(view());
	gui::UIButton& ui_button = gui::GUIFactory::Create<gui::UIButton>(view());
	gui::UICheckBox& ui_checkBox = gui::GUIFactory::Create<gui::UICheckBox>(view());

	void onLoaded() override {
		ui_title.setText(U"This is the example application of OpenSiv3D GUIKit.");
		ui_title.setFont(gui::UnifiedFontStyle::Header);
		ui_title.setDirection(gui::TextDirection::Center);

		ui_button.setText(U"Next");

		ui_title.setConstraint(gui::LayerDirection::Top);
		ui_title.setConstraint(gui::LayerDirection::Height, view(), gui::LayerDirection::Height, 0.0, 0.5);
		ui_title.setConstraint(gui::LayerDirection::Left);
		ui_title.setConstraint(gui::LayerDirection::Right, view(), gui::LayerDirection::Right);

		ui_checkBox.setConstraint(gui::LayerDirection::Height, 100_px);
		ui_checkBox.setConstraint(gui::LayerDirection::Bottom, ui_button, gui::LayerDirection::Top);
		ui_checkBox.setConstraint(gui::LayerDirection::Left);
		ui_checkBox.setConstraint(gui::LayerDirection::Right, view(), gui::LayerDirection::Right);

		ui_button.setConstraint(gui::LayerDirection::Height, 100_px);
		ui_button.setConstraint(gui::LayerDirection::Bottom, view(), gui::LayerDirection::Bottom);
		ui_button.setConstraint(gui::LayerDirection::Left);
		ui_button.setConstraint(gui::LayerDirection::Right, view(), gui::LayerDirection::Right);
		ui_button.addEventListener<gui::MouseEvent::LeftDown>([this] {
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
