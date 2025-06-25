#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

#include "storage/storage_interface.h"

class Settings {
public:
    Settings(const std::string& ns, bool read_write = false);
    ~Settings();

    std::string GetString(const std::string& key, const std::string& default_value = "");
    void SetString(const std::string& key, const std::string& value);
    int32_t GetInt(const std::string& key, int32_t default_value = 0);
    void SetInt(const std::string& key, int32_t value);
    void EraseKey(const std::string& key);
    void EraseAll();

private:
    std::unique_ptr<storage::Storage> storage_manager_;
};

#endif
