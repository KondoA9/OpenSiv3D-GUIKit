#pragma once

#include <Siv3D.hpp>

namespace s3d::CLI {
    // On Windows, the command will be executed on powershell
    bool Execute(StringView command);

#if SIV3D_PLATFORM(WINDOWS)
    enum class WindowType { Hide, Show };

    bool Execute(StringView command,
                 WindowType windowType,
                 bool waitProcessTermination,
                 const Optional<String>& currentDirectory);
#endif

    /// <summary>
    /// Get the content from url and save it to the output path.
    /// If Windows, this function call Invoke-WebRequest command on powershell
    /// </summary>
    /// <param name="createDirectories">Create directiroes to the output path. If there is no path to output, curl
    /// cannot save the content.</param> <returns></returns>
    bool Curl(StringView url, const FilePath& output, bool createDirectories = true);
}
