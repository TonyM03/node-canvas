#pragma once

#include "FontManager.h"
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

class FontManagerLinux : public FontManager {
  public:
    void readSystemFonts(std::vector<FontDescriptor>& results) override;
    void populateFallbackFonts(std::vector<std::string>& families, script_t script) override;
    std::optional<const std::vector<std::string>*> getGenericList(const std::string& generic) override;

  private:
    // FontConfig is lazily initialized the first time we touch it. All of the
    // members below are populated by ensureStartup() and then reused.
    void ensureStartup();
    bool startup_done = false;

    // Color (emoji) and symbol fonts queried once via FC_COLOR / FC_SYMBOL and
    // appended to every fallback cascade, like the other platforms do with
    // "Apple Color Emoji"/"Segoe UI Emoji" and the symbol/dingbat tail.
    std::vector<std::string> emoji_fonts;
    std::vector<std::string> symbol_fonts;

    // "sans-serif"/"serif"/"monospace"/"cursive"/"fantasy" resolved through
    // FcFontMatch so we map to whatever the system actually aliases them to.
    std::unordered_map<std::string, std::vector<std::string>> generic_fonts;

    // Memoized FC_LANG -> covering families, so each language pays for one
    // FcFontSort per process even though populateFallbackFonts runs per run.
    std::unordered_map<std::string, std::vector<std::string>> lang_cache;

    const std::vector<std::string>& langFamilies(const std::string& lang);
};
