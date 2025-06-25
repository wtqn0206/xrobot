#include "storage_interface.h"
#include <iostream>
#ifdef ESP_PLATFORM
#include "esp32/nvs_storage.h"
using namespace storage::esp32;
#else
#include "linux/file_storage.h"
using namespace storage::linux_platform;
#endif

namespace storage {

std::unique_ptr<Storage> StorageFactory::CreateStorage( const std::string& ns, bool read_write){
    std::cout << "[StorageFactory] Creating storage (names=" << ns 
              << ", read_write=" << (read_write ? "true" : "false") << ")" << std::endl;
#ifdef ESP_PLATFORM
    return std::make_unique<Esp32Storage>(ns,read_write);
#else
    return std::make_unique<LinuxStorage>(ns,read_write);
#endif
}


} // namespace storage