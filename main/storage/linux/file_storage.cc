#include <fstream>
#include <iostream>
#include <filesystem>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <sstream> 
#include "file_storage.h"

namespace storage {
namespace linux_storage {

LinuxStorage::LinuxStorage(const std::string& ns, bool read_write) 
    : ns_(ns), read_write_(read_write) {
    
    // 创建命名空间目录
    namespace_dir_ = "/var/lib/myapp/storage/" + ns_;
    if (read_write_) {
        std::filesystem::create_directories(namespace_dir_);
    }
}

LinuxStorage::~LinuxStorage() {
    // 无需清理，文件操作自动关闭
}

std::string LinuxStorage::GetString(const std::string& key, const std::string& default_value) {
    std::string path = get_key_path(key);
    std::ifstream file(path);
    
    if (!file.is_open()) {
        return default_value;
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

StorageError LinuxStorage::SetString(const std::string& key, const std::string& value) {
    if (!read_write_) {
        return StorageError::kFileNotOpen;
    }
    
    std::string path = get_key_path(key);
    std::ofstream file(path);
    
    if (!file.is_open()) {
        return StorageError::kFileNotOpen;
    }
    
    file << value;
    return StorageError::kSuccess;
}

int32_t LinuxStorage::GetInt(const std::string& key, int32_t default_value) {
    std::string value = GetString(key, "");
    if (value.empty()) {
        return default_value;
    }
    
    try {
        return std::stoi(value);
    } catch (...) {
        return default_value;
    }
}

StorageError LinuxStorage::SetInt(const std::string& key, int32_t value) {
    return SetString(key, std::to_string(value));
}

StorageError LinuxStorage::EraseKey(const std::string& key) {
    if (!read_write_) {
        return StorageError::kFileNotOpen;
    }
    
    std::string path = get_key_path(key);
    if (std::filesystem::exists(path)) {
        std::error_code ec;
        std::filesystem::remove(path, ec);
        if (ec) {
            return StorageError::kFailed;
        }
    }
    return StorageError::kSuccess;
}

StorageError LinuxStorage::EraseAll() {
    if (!read_write_) {
        return StorageError::kFileNotOpen;
    }
    
    std::error_code ec;
    std::filesystem::remove_all(namespace_dir_, ec);
    if (ec) {
        return StorageError::kFailed;
    }
    
    // 重新创建命名空间目录
    std::filesystem::create_directories(namespace_dir_);
    return StorageError::kSuccess;
}

std::string LinuxStorage::get_key_path(const std::string& key) const {
    // 替换不合法的文件名字符
    std::string safe_key = key;
    for (char& c : safe_key) {
        if (!std::isalnum(c) && c != '_' && c != '-') {
            c = '_';
        }
    }
    return namespace_dir_ + "/" + safe_key;
}

} // namespace linux
} // namespace platform