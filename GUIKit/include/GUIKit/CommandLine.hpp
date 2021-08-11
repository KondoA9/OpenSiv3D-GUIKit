#pragma once

#include <Siv3D.hpp>

namespace s3d::CLI {
	// On Windows, the command will be executed on powershell
	bool Execute(const String& command);

	// Open path on explorer or finder
	bool OpenFolder(const String& path);

	// Get item from url to output
	bool Curl(const String& url, const FilePath& output);
}