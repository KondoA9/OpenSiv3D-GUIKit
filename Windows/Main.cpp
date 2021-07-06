#include <GUIKit/GUIKit.h>

class ToolBar final : public gui::UIView {
	std::function<void()> m_toggleColorModeHandler;
	std::function<void(const FilePath&)> m_folderOpenedHandler;
	std::function<void()> m_openParentDirHandler;

	gui::UIButton ui_openDirectoryButton = gui::UIButton(U"Open");
	gui::UIButton ui_parentDirButton = gui::UIButton(Texture(Icon(0xf062, 20)));
	gui::UIToggleButton ui_toggleColorModeButton = gui::UIToggleButton(Texture(Icon(0xf186, 20)));

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

		appendComponent(ui_toggleColorModeButton);
		appendComponent(ui_parentDirButton);
		appendComponent(ui_openDirectoryButton);
	}
};

class FileView final : public gui::UIView {
	FilePath m_path;
	std::function<void(const FilePath& path)> m_folderSelectedHandler;

	gui::UIIcon ui_icon;
	gui::UIText ui_fileName;
	gui::UIText ui_updatedDate;
	gui::UIText ui_kind;

public:
	FileView(const FilePath& path)
		:UIView(),
		m_path(path),
		ui_icon(FileSystem::IsDirectory(path) ? Icon(0xf07b, 20) : Icon(0xf15b, 20), FileSystem::IsDirectory(path) ? gui::ColorTheme(Color(246, 218, 75)) : gui::DynamicColor::Text),
		ui_fileName(gui::UIText(FileSystem::FileName(path), gui::UnifiedFontStyle::Small)),
		ui_updatedDate(gui::UIText(gui::UnifiedFontStyle::Small, gui::DynamicColor::Background)),
		ui_kind(gui::UIText(FileSystem::IsDirectory(path) ? U"フォルダ" : U"ファイル", gui::UnifiedFontStyle::Small, gui::DynamicColor::Background))
	{
		const auto time = FileSystem::WriteTime(path);
		if (time.has_value()) {
			ui_updatedDate.text = time.value().format();
		}
	}

	void setFolderSelectedHandler(const std::function<void(const FilePath& path)>& handler) {
		m_folderSelectedHandler = handler;
	}

protected:
	void initialize() override {
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

		appendComponent(ui_icon);
		appendComponent(ui_fileName);
		appendComponent(ui_updatedDate);
		appendComponent(ui_kind);
	}
};

class StartPage : public gui::Page {
	using Page::Page;

	FilePath m_path;

	ToolBar ui_toolbar = ToolBar(gui::DynamicColor::BackgroundSecondary);
	gui::UIVStackView ui_filesView = gui::UIVStackView();
	gui::UIButton ui_movePage = gui::UIButton(U"Move page");

	void onLoaded() override {
		ui_toolbar.drawFrame = true;
		ui_toolbar.setConstraint(gui::LayerDirection::Top, m_view, gui::LayerDirection::Top);
		ui_toolbar.setConstraint(gui::LayerDirection::Height, 30_px);
		ui_toolbar.setConstraint(gui::LayerDirection::Left);
		ui_toolbar.setConstraint(gui::LayerDirection::Right, m_view, gui::LayerDirection::Right);
		ui_toolbar.setToggleColorModeHandler([this] {
			gui::GUIKit::Instance().toggleColorMode();
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
		ui_filesView.setConstraint(gui::LayerDirection::Left, m_view, gui::LayerDirection::Left);
		ui_filesView.setConstraint(gui::LayerDirection::Right, m_view, gui::LayerDirection::Right);

		ui_movePage.setConstraint(gui::LayerDirection::Height, 40_px);
		ui_movePage.setConstraint(gui::LayerDirection::Bottom, m_view, gui::LayerDirection::Bottom);
		ui_movePage.setConstraint(gui::LayerDirection::Left, m_view, gui::LayerDirection::Left);
		ui_movePage.setConstraint(gui::LayerDirection::Right, m_view, gui::LayerDirection::Right);
		ui_movePage.addEventListener<gui::MouseEvent::LeftDown>([this] {
			gui::GUIKit::Instance().switchPage(U"start");
			});

		m_view.appendComponent(ui_filesView);
		m_view.appendComponent(ui_movePage);
		m_view.appendComponent(ui_toolbar);
	}

	void setup(const FilePath& dir) {
		m_path = dir;

		ui_filesView.release();

		const auto contents = FileSystem::DirectoryContents(dir, false);
		for (const auto& c : contents) {
			auto row = FileView(c);
			row.penetrateMouseEvent = true;
			row.setFolderSelectedHandler([this](FilePath path) {
				setup(path);
				});
			ui_filesView.appendTemporaryComponent(row);
		}
	}
};

class NextPage : public gui::Page {
	using Page::Page;

	gui::UIText ui_title = gui::UIText(U"GUIKit explorer", gui::UnifiedFontStyle::Large, gui::TextDirection::Center);
	gui::UIButton ui_button = gui::UIButton(U"Next");

	void onLoaded() override {
		ui_title.setConstraint(gui::LayerDirection::Top);
		ui_title.setConstraint(gui::LayerDirection::Height, m_view, gui::LayerDirection::Height, 0.0, 0.5);
		ui_title.setConstraint(gui::LayerDirection::Left);
		ui_title.setConstraint(gui::LayerDirection::Right, m_view, gui::LayerDirection::Right);

		ui_button.setConstraint(gui::LayerDirection::Top, ui_title, gui::LayerDirection::Bottom);
		ui_button.setConstraint(gui::LayerDirection::Bottom, m_view, gui::LayerDirection::Bottom);
		ui_button.setConstraint(gui::LayerDirection::Left);
		ui_button.setConstraint(gui::LayerDirection::Right, m_view, gui::LayerDirection::Right);
		ui_button.addEventListener<gui::MouseEvent::LeftDown>([this] {
			gui::GUIKit::Instance().switchPage(U"explorer");
			});

		m_view.appendComponent(ui_title);
		m_view.appendComponent(ui_button);
	}
};

void Main() {
	auto& guikit = gui::GUIKit::Instance();

	Window::Resize(1920, 1080);

	guikit.appendPage(NextPage(U"start"));
	guikit.appendPage(StartPage(U"explorer"));
	guikit.start();
}
