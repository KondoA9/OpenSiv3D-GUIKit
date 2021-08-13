#pragma once

#include <Siv3D.hpp>

namespace s3d::CLI {
	// On Windows, the command will be executed on powershell
	bool Execute(const String& command);

	// Open path on explorer or finder
	bool OpenFolder(const String& path);

	// If Windows, curl is the alias of Invoke-WebRequest
	bool Curl(const String& url, const FilePath& output);
}