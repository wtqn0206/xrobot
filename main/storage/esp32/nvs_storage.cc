#include "nvs_storage.h"
#include <nvs_flash.h>
#include <sstream>
#include <iomanip>

namespace storage {
namespace esp32 {

// Esp32Storage 实现
Esp32Storage::Esp32Storage(const std::string& ns, bool read_write) : ns_(ns), read_write_(read_write) {
    nvs_open(ns.c_str(), read_write_ ? NVS_READWRITE : NVS_READONLY, &nvs_handle_);
}

Esp32Storage::~Esp32Storage() {
    if (nvs_handle_ != 0) {
        if (read_write_ && dirty_) {
            ESP_ERROR_CHECK(nvs_commit(nvs_handle_));
        }
        nvs_close(nvs_handle_);
    }
}

std::string Esp32Storage::GetString(const std::string& key, const std::string& default_value)
{
    if (nvs_handle_ == 0) {
        return default_value;
    }

    size_t length = 0;
    if (nvs_get_str(nvs_handle_, key.c_str(), nullptr, &length) != ESP_OK) {
        return default_value;
    }

    std::string value;
    value.resize(length);
    ESP_ERROR_CHECK(nvs_get_str(nvs_handle_, key.c_str(), value.data(), &length));
    while (!value.empty() && value.back() == '\0') {
        value.pop_back();
    }
    return value;
}

StorageError Esp32Storage::SetString(const std::string& key, const std::string& value)
{
    if (read_write_) {
        ESP_ERROR_CHECK(nvs_set_str(nvs_handle_, key.c_str(), value.c_str()));
        dirty_ = true;
        return StorageError::kSuccess;
    } else {
        //ESP_LOGW(TAG, "Namespace %s is not open for writing", ns_.c_str());
        return StorageError::kFileNotOpen;
    }
}

int32_t Esp32Storage::GetInt(const std::string& key, int32_t default_value)
{
    if (nvs_handle_ == 0) {
        return default_value;
    }

    int32_t value;
    if (nvs_get_i32(nvs_handle_, key.c_str(), &value) != ESP_OK) {
        return default_value;
    }
    return value;
}

StorageError Esp32Storage::SetInt(const std::string& key, int32_t value)
{
    if (read_write_) {
        ESP_ERROR_CHECK(nvs_set_i32(nvs_handle_, key.c_str(), value));
        dirty_ = true;
        return StorageError::kSuccess;
    } else {
        //ESP_LOGW(TAG, "Namespace %s is not open for writing", ns_.c_str());
        return StorageError::kFileNotOpen;
    }
}

StorageError Esp32Storage::EraseKey(const std::string& key)
{
    if (read_write_) {
        auto ret = nvs_erase_key(nvs_handle_, key.c_str());
        if (ret != ESP_ERR_NVS_NOT_FOUND) {
            ESP_ERROR_CHECK(ret);
        }
        return StorageError::kSuccess;
    } else {
        //ESP_LOGW(TAG, "Namespace %s is not open for writing", ns_.c_str());
        return StorageError::kFileNotOpen;
    }
}

StorageError Esp32Storage::EraseAll()
{
    if (read_write_) {
        ESP_ERROR_CHECK(nvs_erase_all(nvs_handle_));
        return StorageError::kSuccess;
    } else {
        //ESP_LOGW(TAG, "Namespace %s is not open for writing", ns_.c_str());
        return StorageError::kFileNotOpen;
    }
}

} // namespace esp32
} // namespace storage