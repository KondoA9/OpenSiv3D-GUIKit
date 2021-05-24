#include <GUIKit.h>

class StartMenuUI : public gui::Page {
public:
	using Page::Page;

	gui::UIText title = gui::UIText(U"GUIKit App", gui::TextDirection::LeftTop);
	gui::UIButton button = gui::UIButton(U"Edit");
	gui::UIButton toggle = gui::UIButton(U"Toggle");

	Array<gui::UIText> texts;
	gui::UIVStackView vStackView = gui::UIVStackView(Palette::Red);
	
	void onLoaded() override {
		title.setConstraint(gui::LayerDirection::Top);
		title.setConstraint(gui::LayerDirection::Height, 100);
		title.setConstraint(gui::LayerDirection::Left);
		title.setConstraint(gui::LayerDirection::Right, view, gui::LayerDirection::Right, 1.0, 1.0);

		button.setConstraint(gui::LayerDirection::Top, title, gui::LayerDirection::Bottom);
		button.setConstraint(gui::LayerDirection::Height, 300);
		button.setConstraint(gui::LayerDirection::Left);
		button.setConstraint(gui::LayerDirection::Right, view, gui::LayerDirection::Right, 1.0, 0.5);
		button.addEventListener(gui::MouseEvent::Clicked, [this]() {
			guikit->switchPage(U"Editor");
			});

		toggle.setConstraint(gui::LayerDirection::Top, button, gui::LayerDirection::Top);
		toggle.setConstraint(gui::LayerDirection::Bottom, button, gui::LayerDirection::Bottom);
		toggle.setConstraint(gui::LayerDirection::Left, button, gui::LayerDirection::Right);
		toggle.setConstraint(gui::LayerDirection::Width, button, gui::LayerDirection::Width);
		toggle.addEventListener(gui::MouseEvent::Clicked, [this]() {
			guikit->toggleColorMode();
			});

		vStackView.setConstraint(gui::LayerDirection::Top, toggle, gui::LayerDirection::Bottom);
		vStackView.setConstraint(gui::LayerDirection::Bottom, view, gui::LayerDirection::Bottom);
		vStackView.setConstraint(gui::LayerDirection::Left);
		vStackView.setConstraint(gui::LayerDirection::Right, view, gui::LayerDirection::Right);
		vStackView.setMaxStackCount(6);
		for (size_t i : step(4)) {
			texts.push_back(gui::UIText(ToString(i), gui::TextDirection::Center));
			vStackView.appendComponent(texts[i]);
		}

		view.appendComponent(title);
		view.appendComponent(button);
		view.appendComponent(toggle);
		view.appendComponent(vStackView);
	}
};

class EditorUI : public gui::Page {
	String images[2] = { U"image.jpg", U"image.png" };
	Circle pen;
	bool toggle = true;

	gui::UICircle cursor = gui::UICircle();
	gui::UIImage editorComponent = gui::UIImage(Palette::White);
	gui::UIRect toolbarComponent = gui::UIRect();
	gui::UIButton backButton = gui::UIButton(U"Back to start menu");
	gui::UIButton trashButton = gui::UIButton(U"Trash");
	gui::UIButton toggleImageButton = gui::UIButton(U"Toggle image");

public:
	using Page::Page;

	void onLoaded() override {
		cursor.backgroundColor = Palette::Darkgreen;
		cursor.setConstraint(gui::LayerDirection::Height, 50);
		cursor.setConstraint(gui::LayerDirection::Width, 50);

		editorComponent.setConstraint(gui::LayerDirection::Top);
		editorComponent.setConstraint(gui::LayerDirection::Left);
		editorComponent.setConstraint(gui::LayerDirection::Bottom, view, gui::LayerDirection::Bottom, 0, 0.8);
		editorComponent.setConstraint(gui::LayerDirection::Right, view, gui::LayerDirection::Right);
		editorComponent.addEventListener(gui::MouseEvent::Hovered, [this]() {
			cursor.setConstraint(gui::LayerDirection::CenterY, Cursor::Pos().y);
			cursor.setConstraint(gui::LayerDirection::CenterX, Cursor::Pos().x);
			Cursor::RequestStyle(CursorStyle::Cross);
			});
		editorComponent.addEventListener(gui::MouseEvent::Dragged, [this]() {
			editorComponent.paint(50, cursor.backgroundColor);
			});

		toolbarComponent.setConstraint(gui::LayerDirection::Top, editorComponent, gui::LayerDirection::Bottom);
		toolbarComponent.setConstraint(gui::LayerDirection::Bottom, view, gui::LayerDirection::Bottom);
		toolbarComponent.setConstraint(gui::LayerDirection::Left);
		toolbarComponent.setConstraint(gui::LayerDirection::Right, view, gui::LayerDirection::Right);

		backButton.setConstraint(gui::LayerDirection::Top, toolbarComponent, gui::LayerDirection::Top);
		backButton.setConstraint(gui::LayerDirection::Bottom, toolbarComponent, gui::LayerDirection::Bottom);
		backButton.setConstraint(gui::LayerDirection::Left);
		backButton.setConstraint(gui::LayerDirection::Right, editorComponent, gui::LayerDirection::Right, 0, 0.2);
		backButton.addEventListener(gui::MouseEvent::Clicked, [this]() {
			guikit->switchPage(U"Start");
			});

		trashButton.setConstraint(gui::LayerDirection::Top, backButton, gui::LayerDirection::Top);
		trashButton.setConstraint(gui::LayerDirection::Bottom, backButton, gui::LayerDirection::Bottom);
		trashButton.setConstraint(gui::LayerDirection::Left, backButton, gui::LayerDirection::Right);
		trashButton.setConstraint(gui::LayerDirection::Width, backButton, gui::LayerDirection::Width);
		trashButton.addEventListener(gui::MouseEvent::Clicked, [this]() {
			editorComponent.setImage(Image(images[toggle ? 0 : 1]));
			});

		toggleImageButton.setConstraint(gui::LayerDirection::Top, backButton, gui::LayerDirection::Top);
		toggleImageButton.setConstraint(gui::LayerDirection::Bottom, backButton, gui::LayerDirection::Bottom);
		toggleImageButton.setConstraint(gui::LayerDirection::Left, trashButton, gui::LayerDirection::Right);
		toggleImageButton.setConstraint(gui::LayerDirection::Width, trashButton, gui::LayerDirection::Width);
		toggleImageButton.addEventListener(gui::MouseEvent::Clicked, [this]() {
			editorComponent.setImage(Image(images[toggle ? 1 : 0]));
			toggle = !toggle;
			});

		view.appendComponent(editorComponent);
		view.appendComponent(cursor);
		view.appendComponent(toolbarComponent);
		view.appendComponent(backButton);
		view.appendComponent(trashButton);
		view.appendComponent(toggleImageButton);
	}

	void onAfrerAppeared() override {
		editorComponent.setImage(Image(images[0]));
	}
};


void Main() {
	gui::GUIKit guikit;

	LicenseManager::DisableDefaultTrigger();
	Scene::SetTextureFilter(TextureFilter::Nearest);
	guikit.setTitle(U"GUIKit Paint");

	auto startMenu = StartMenuUI(U"Start");
	auto editor = EditorUI(U"Editor");

	guikit.appendPage(startMenu);
	guikit.appendPage(editor);

	guikit.start();
}