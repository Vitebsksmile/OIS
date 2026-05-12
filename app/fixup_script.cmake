include(BundleUtilities)

# Пути для поиска DLL (в MSYS2 всё лежит в /bin)
set(DIRS 
    "D:/msys64/mingw64/bin"
    "D:/msys64/mingw64/share/qt6/plugins/platforms" # Для qwindows.dll
)

# Запуск анализа и копирования
# Скрипт проверит OISApp.exe, увидит, что ему нужна libopencv_core.dll, 
# найдет её в /mingw64/bin, скопирует и проверит, что нужно самой этой DLL.
fixup_bundle("${EXE_PATH}" "" "${DIRS}")
#fixup_bundle("${EXE_PATH}" "${QML_PLUGINS}" "${DIRS}")