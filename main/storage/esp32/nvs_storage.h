#pragma once

#include "../storage_interface.h"
#include <nvs_flash.h>


namespace storage {
namespace esp32 {

class Esp32Storage : public Storage {
public:
     Esp32Storage(const std::string& ns, bool read_write = false);
    ~Esp32Storage() override;
    
    std::string GetString(const std::string& key, const std::string& default_value = "") override;
    StorageError SetString(const std::string& key, const std::string& value) override;
    int32_t GetInt(const std::string& key, int32_t default_value = 0) override;
    StorageError SetInt(const std::string& key, int32_t value) override;
    StorageError EraseKey(const std::string& key) override;
    StorageError EraseAll() override;
    
private:
    std::string ns_;
    nvs_handle_t nvs_handle_ = 0;
    bool read_write_ = false;
    bool dirty_ = false;
};

} // namespace esp32
} // namespace storage