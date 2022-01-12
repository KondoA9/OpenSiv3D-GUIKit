#include <Aoba/Aoba.hpp>

class ToolBar final : public aoba::UIView {
private:
	std::function<void()> m_toggleColorModeHandler;
	std::function<void(const FilePath&)> m_folderOpenedHandler;
	std::function<void()> m_openParentDirHandler;

	aoba::UIToggleButton& m_uiToggleColorModeButton = aoba::Factory::Create<aoba::UIToggleButton>(this);
	aoba::UIButton& m_uiParentDirButton = aoba::Factory::Create<aoba::UIButton>(this);
	aoba::UIButton& m_uiOpenDirectoryButton = aoba::Factory::Create<aoba::UIButton>(this);

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

		m_uiToggleColorModeButton.setConstraint(aoba::LayerDirection::Top, *this, aoba::LayerDirection::Top);
		m_uiToggleColorModeButton.setConstraint(aoba::LayerDirection::Bottom, *this, aoba::LayerDirection::Bottom);
		m_uiToggleColorModeButton.setConstraint(aoba::LayerDirection::Left);
		m_uiToggleColorModeButton.setConstraint(aoba::LayerDirection::Width, 40_px);
		m_uiToggleColorModeButton.addEventListener<aoba::MouseEvent::LeftDown>([this] {
			m_toggleColorModeHandler();
			});

		m_uiParentDirButton.setConstraint(aoba::LayerDirection::Top, *this, aoba::LayerDirection::Top);
		m_uiParentDirButton.setConstraint(aoba::LayerDirection::Bottom, *this, aoba::LayerDirection::Bottom);
		m_uiParentDirButton.setConstraint(aoba::LayerDirection::Left, m_uiToggleColorModeButton, aoba::LayerDirection::Right);
		m_uiParentDirButton.setConstraint(aoba::LayerDirection::Width, 40_px);
		m_uiParentDirButton.addEventListener<aoba::MouseEvent::LeftDown>([this] {
			m_openParentDirHandler();
			});

		m_uiOpenDirectoryButton.setConstraint(aoba::LayerDirection::Top, *this, aoba::LayerDirection::Top);
		m_uiOpenDirectoryButton.setConstraint(aoba::LayerDirection::Bottom, *this, aoba::LayerDirection::Bottom);
		m_uiOpenDirectoryButton.setConstraint(aoba::LayerDirection::Left, m_uiParentDirButton, aoba::LayerDirection::Right);
		m_uiOpenDirectoryButton.setConstraint(aoba::LayerDirection::Width, 120_px);
		m_uiOpenDirectoryButton.addEventListener<aoba::MouseEvent::LeftDown>([this] {
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

	aoba::UIIcon& m_uiIcon = aoba::Factory::Create<aoba::UIIcon>(this);
	aoba::UIText& m_uiFileName = aoba::Factory::Create<aoba::UIText>(this);
	aoba::UIText& m_uiUpdatedDate = aoba::Factory::Create<aoba::UIText>(this);
	aoba::UIText& m_uiKind = aoba::Factory::Create<aoba::UIText>(this);

public:
	FileView()
		:UIView()
	{
		m_uiUpdatedDate.backgroundColor = aoba::DynamicColor::Background;
		m_uiKind.backgroundColor = aoba::DynamicColor::Background;
	}

	void setPath(const FilePath& path) {
		m_uiIcon.setIcon(FileSystem::IsDirectory(path) ? Icon(0xf07b) : Icon(0xf15b), 20_px);
		m_uiIcon.iconColor = FileSystem::IsDirectory(path) ? aoba::ColorTheme(Color(246, 218, 75)) : aoba::DynamicColor::Text;

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

		addEventListener<aoba::MouseEvent::Hovered>([this] {
			backgroundColor.highlight(aoba::DynamicColor::BackgroundSecondary);
			});
		addEventListener<aoba::MouseEvent::UnHovered>([this] {
			backgroundColor.lowlight(aoba::DynamicColor::Background);
			});
		addEventListener<aoba::MouseEvent::LeftDown>([this] {
			if (FileSystem::IsDirectory(m_path)) {
				m_folderSelectedHandler(m_path);
			}
			});

		m_uiIcon.penetrateMouseEvent = true;
		m_uiIcon.setConstraint(aoba::LayerDirection::Top, *this, aoba::LayerDirection::Top);
		m_uiIcon.setConstraint(aoba::LayerDirection::Bottom, *this, aoba::LayerDirection::Bottom);
		m_uiIcon.setConstraint(aoba::LayerDirection::Left, *this, aoba::LayerDirection::Left);
		m_uiIcon.setConstraint(aoba::LayerDirection::Width, *this, aoba::LayerDirection::Height);

		m_uiFileName.penetrateMouseEvent = true;
		m_uiFileName.setConstraint(aoba::LayerDirection::Top, *this, aoba::LayerDirection::Top);
		m_uiFileName.setConstraint(aoba::LayerDirection::Bottom, *this, aoba::LayerDirection::Bottom);
		m_uiFileName.setConstraint(aoba::LayerDirection::Left, m_uiIcon, aoba::LayerDirection::Right);
		m_uiFileName.setConstraint(aoba::LayerDirection::Right, m_uiUpdatedDate, aoba::LayerDirection::Left);

		m_uiUpdatedDate.penetrateMouseEvent = true;
		m_uiUpdatedDate.setPadding(0, 0, 5_px, 5_px);
		m_uiUpdatedDate.setConstraint(aoba::LayerDirection::Top, *this, aoba::LayerDirection::Top);
		m_uiUpdatedDate.setConstraint(aoba::LayerDirection::Bottom, *this, aoba::LayerDirection::Bottom);
		m_uiUpdatedDate.setConstraint(aoba::LayerDirection::Width, 220_px);
		m_uiUpdatedDate.setConstraint(aoba::LayerDirection::Right, m_uiKind, aoba::LayerDirection::Left);

		m_uiKind.penetrateMouseEvent = true;
		m_uiKind.setPadding(0, 0, 5_px, 5_px);
		m_uiKind.setConstraint(aoba::LayerDirection::Top, *this, aoba::LayerDirection::Top);
		m_uiKind.setConstraint(aoba::LayerDirection::Bottom, *this, aoba::LayerDirection::Bottom);
		m_uiKind.setConstraint(aoba::LayerDirection::Width, 80_px);
		m_uiKind.setConstraint(aoba::LayerDirection::Right, *this, aoba::LayerDirection::Right);
	}
};

class ExplorerPage : public aoba::Page {
private:
	using Page::Page;

	FilePath m_path;

	ToolBar& m_uiToolbar = aoba::Factory::Create<ToolBar>(view);
	aoba::UIVStackView& m_uiFilesView = aoba::Factory::Create<aoba::UIVStackView>(view);
	aoba::UIButton& m_uiMovePage = aoba::Factory::Create<aoba::UIButton>(view);

protected:
	void onLoaded() override {
		m_uiToolbar.backgroundColor = aoba::DynamicColor::BackgroundSecondary;
		m_uiMovePage.setText(U"Move page");

		m_uiToolbar.drawFrame = true;
		m_uiToolbar.setConstraint(aoba::LayerDirection::Top, view, aoba::LayerDirection::Top);
		m_uiToolbar.setConstraint(aoba::LayerDirection::Height, 30_px);
		m_uiToolbar.setConstraint(aoba::LayerDirection::Left);
		m_uiToolbar.setConstraint(aoba::LayerDirection::Right, view, aoba::LayerDirection::Right);
		m_uiToolbar.setToggleColorModeHandler([this] {
			aoba::Core::ToggleColorMode();
			});
		m_uiToolbar.setFolderOpendHandler([this](const FilePath& path) {
			setup(path);
			});
		m_uiToolbar.setOpenParentDirHandler([this] {
			const FilePath path = FileSystem::ParentPath(m_path);
			setup(path);
			});

		m_uiFilesView.setLeadingDirection(aoba::LeadingDirection::Bottom);
		m_uiFilesView.setRowHeight(30_px);
		m_uiFilesView.setVerticalMargin(20_px);
		m_uiFilesView.setConstraint(aoba::LayerDirection::Top, m_uiToolbar, aoba::LayerDirection::Bottom);
		m_uiFilesView.setConstraint(aoba::LayerDirection::Bottom, m_uiMovePage, aoba::LayerDirection::Top);
		m_uiFilesView.setConstraint(aoba::LayerDirection::Left, view, aoba::LayerDirection::Left);
		m_uiFilesView.setConstraint(aoba::LayerDirection::Right, view, aoba::LayerDirection::Right);

		m_uiMovePage.setConstraint(aoba::LayerDirection::Height, 40_px);
		m_uiMovePage.setConstraint(aoba::LayerDirection::Bottom, view, aoba::LayerDirection::Bottom);
		m_uiMovePage.setConstraint(aoba::LayerDirection::Left, view, aoba::LayerDirection::Left);
		m_uiMovePage.setConstraint(aoba::LayerDirection::Right, view, aoba::LayerDirection::Right);
		m_uiMovePage.addEventListener<aoba::MouseEvent::LeftDown>([this] {
			aoba::Core::SwitchPage(U"start");
			});
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
			row.setFolderSelectedHandler([this](FilePath path) {
				setup(path);
				});
		}
	}
};

class StartPage : public aoba::Page {
private:
	using Page::Page;

	aoba::UIText& m_uiTitle = aoba::Factory::Create<aoba::UIText>(view);
	aoba::UIInputField& m_uiInputField = aoba::Factory::Create<aoba::UIInputField>(view);
	aoba::UIButton& m_uiButton = aoba::Factory::Create<aoba::UIButton>(view);

protected:
	void onLoaded() override {
		Page::onLoaded();

		m_uiTitle.setText(U"This is the example application of Aoba.");
		m_uiTitle.setFont(aoba::UnifiedFontStyle::Header);
		m_uiTitle.setDirection(aoba::TextDirection::Center);

		m_uiButton.setText(U"Next");

		m_uiTitle.setConstraint(aoba::LayerDirection::Top);
		m_uiTitle.setConstraint(aoba::LayerDirection::Height, view, aoba::LayerDirection::Height, 0.0, 0.5);
		m_uiTitle.setConstraint(aoba::LayerDirection::Left);
		m_uiTitle.setConstraint(aoba::LayerDirection::Right, view, aoba::LayerDirection::Right);

		m_uiInputField.setText(U"This is the example application of Aoba.");
		m_uiInputField.setConstraint(aoba::LayerDirection::Height, 100_px);
		m_uiInputField.setConstraint(aoba::LayerDirection::Bottom, m_uiButton, aoba::LayerDirection::Top);
		m_uiInputField.setConstraint(aoba::LayerDirection::Left);
		m_uiInputField.setConstraint(aoba::LayerDirection::Right, view, aoba::LayerDirection::Right);

		m_uiButton.setConstraint(aoba::LayerDirection::Height, 100_px);
		m_uiButton.setConstraint(aoba::LayerDirection::Bottom, view, aoba::LayerDirection::Bottom);
		m_uiButton.setConstraint(aoba::LayerDirection::Left);
		m_uiButton.setConstraint(aoba::LayerDirection::Right, view, aoba::LayerDirection::Right);
		m_uiButton.addEventListener<aoba::MouseEvent::LeftDown>([this] {
			aoba::Core::SwitchPage(U"explorer");
			});
	}
};

void AobaMain() {
	Window::Resize(1280, 720);

	aoba::Core::AppendPage<StartPage>(U"start");
	aoba::Core::AppendPage<ExplorerPage>(U"explorer");

	aoba::Core::Start();
}
