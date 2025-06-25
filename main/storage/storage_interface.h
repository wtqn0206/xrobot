#pragma once

#include <functional>
#include <memory>
#include <string>
#include <cstdint>

namespace storage {

// 平台无关的错误码定义
enum class StorageError {
    kSuccess = 0,
    kFailed = -1,
    kInvalidParameter = -2,
    kFileNotOpen = -3,
    kTimeout = -4,
    kNotSupported = -5
};



// 文件存储接口
class Storage {
public:
    virtual ~Storage() = default;
    //virtual StorageError Open() = 0;
    virtual std::string GetString(const std::string& key, const std::string& default_value = "") = 0;
    virtual StorageError SetString(const std::string& key, const std::string& value) = 0;
    virtual int32_t GetInt(const std::string& key, int32_t default_value = 0) = 0;
    virtual StorageError SetInt(const std::string& key, int32_t value) = 0;
    virtual StorageError EraseKey(const std::string& key) = 0;
    virtual StorageError EraseAll() = 0;
};


// 系统工厂类
class StorageFactory {
public:
    static std::unique_ptr<Storage> CreateStorage(const std::string& ns, bool read_write = false);
};

} // namespace storage