﻿#pragma once

#include <Siv3D.hpp>

namespace s3d::aoba {
	class UIView;
	class Core;
	class PageManager;

	class Page {
		friend Core;
		friend PageManager;

	public:
		UIView& view;

	private:
		const String m_identifier;
		bool m_loaded = false;
		bool m_acceptDragDrop = false;

	public:
		const String& identifier() const {
			return m_identifier;
		}

		bool didLoaded() const {
			return m_loaded;
		}

		bool isDragDropAccepted() const {
			return m_acceptDragDrop;
		}

		void acceptDragDrop(bool accept);

	protected:
		virtual ~Page() = default;

		// Called when Aoba core system loaded the page
		virtual void onLoaded();

		// Called before Aoba core system draws the page
		virtual void onBeforeAppeared();

		// Called after Aoba core system draws the page
		virtual void onAfterAppeared();

		// Called before Aoba core system erases the page
		virtual void onBeforeDisappeared();

		// Called after Aoba core system erases the page
		virtual void onAfterDisappeared();

		// Called when window resized
		virtual void onWindowResized();

		// Called before when application terminated
		virtual void onBeforeAppTerminated();

		// Called when application terminated
		virtual void onAppTerminated();

		// Called when the files or texts are drag and dropped
		virtual void onDragDrop(const Array<DroppedFilePath>& files, const Array<DroppedText>& texts);

	private:
		explicit Page(const String& identifier);
	};
}
