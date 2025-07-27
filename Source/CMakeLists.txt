# -----------------------------------------------------------------------------
# Add module directories
# -----------------------------------------------------------------------------
add_subdirectory(mimalloc)
add_subdirectory(WTF)
add_subdirectory(WebCore)

WEBKIT_INCLUDE_CONFIG_FILES_IF_EXISTS()
