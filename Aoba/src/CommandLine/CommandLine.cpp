#include <Aoba/CommandLine.hpp>

#if SIV3D_PLATFORM(WINDOWS)
#include <Siv3D/Windows/Windows.hpp>
#endif

namespace s3d::CLI {
	bool Execute(const String& command) {
#if SIV3D_PLATFORM(WINDOWS)
		STARTUPINFO  si;
		PROCESS_INFORMATION pi;

		memset(&si, 0, sizeof(si));
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;

		std::wstring cmd = (U"powershell " + command).toWstr();
		const auto result = CreateProcess(NULL, cmd.data(), NULL, NULL, FALSE,
			CREATE_DEFAULT_ERROR_MODE | NORMAL_PRIORITY_CLASS,
			NULL, NULL, &si, &pi);

		const auto hWndmain = pi.hProcess;
		CloseHandle(pi.hThread);

		WaitForSingleObject(hWndmain, INFINITE);
		CloseHandle(hWndmain);

		return static_cast<bool>(result);

#else
		return system(command.narrow().c_str()) == 0;
#endif

		return false;
	}

	bool Curl(const String& url, const FilePath& output, bool createDirectories) {
		bool result = true;

		if (createDirectories) {
			FileSystem::CreateDirectories(FileSystem::ParentPath(output));
		}

		if (FileSystem::Exists(output)) {
			FileSystem::Remove(output);
		}

#if SIV3D_PLATFORM(WINDOWS)
		result &= Execute(U"Invoke-WebRequest \\\"{}\\\" -OutFile \\\"{}\\\" -UseBasicParsing"_fmt(url, output));
#else
		result &= Execute(U"curl {} -o {}"_fmt(url, output));
#endif

		return result && FileSystem::Exists(output);
	}
}
