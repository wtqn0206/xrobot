#include "settings.h"

#include <esp_log.h>
#include "storage/storage_interface.h"

#define TAG "Settings"

Settings::Settings(const std::string& ns, bool read_write) {
    storage_manager_ = storage::StorageFactory::CreateStorage(ns, read_write);
}

Settings::~Settings() {

}

std::string Settings::GetString(const std::string& key, const std::string& default_value) {

    return storage_manager_->GetString(key,default_value);
}

void Settings::SetString(const std::string& key, const std::string& value) {

    if (storage::StorageError::kSuccess != storage_manager_->SetString(key,value))
        printf("ERROR: Settings SetString Falied\n");
}

int32_t Settings::GetInt(const std::string& key, int32_t default_value) {

    return storage_manager_->GetInt(key,default_value);
}

void Settings::SetInt(const std::string& key, int32_t value) {
    if (storage::StorageError::kSuccess != storage_manager_->SetInt(key,value))
        printf("ERROR: Settings SetInt Falied\n");
}

void Settings::EraseKey(const std::string& key) {

    if (storage::StorageError::kSuccess != storage_manager_->EraseKey(key))
        printf("ERROR: Settings EraseKey Falied\n");
}

void Settings::EraseAll() {
    if (storage::StorageError::kSuccess != storage_manager_->EraseAll())
        printf("ERROR: Settings EraseAll Falied\n");
}
