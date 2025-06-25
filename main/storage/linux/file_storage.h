#pragma once

#include "../storage_interface.h"
#include <fstream>
#include <iostream>
#include <filesystem>


namespace storage {
namespace linux_storage {

class LinuxStorage : public Storage {
public:
     LinuxStorage(const std::string& ns, bool read_write = false);
    ~LinuxStorage() override;
    
    std::string GetString(const std::string& key, const std::string& default_value = "") override;
    StorageError SetString(const std::string& key, const std::string& value) override;
    int32_t GetInt(const std::string& key, int32_t default_value = 0) override;
    StorageError SetInt(const std::string& key, int32_t value) override;
    StorageError EraseKey(const std::string& key) override;
    StorageError EraseAll() override;
    
private:
    std::string ns_;
    bool read_write_;
    std::string namespace_dir_;

    std::string get_key_path(const std::string& key) const;
};

} // namespace linux_storage
} // namespace storage
