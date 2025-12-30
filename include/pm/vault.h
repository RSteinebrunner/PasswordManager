#pragma once

#include <filesystem>

enum class vaultErr : int{
    Success = 0,
    NoPath = 1,
    VaultExists = 2,
    VaultCreateFailed = 3,
    VaultWriteFailed = 4,
    VaultReadFailed = 5
};

class vault{
    public:
        vault();
        explicit vault(const std::filesystem::path& newPath);
        vaultErr setDefaultFilepath(const std::filesystem::path& newDefaultPath);
        vaultErr initVault(const bool& force);

    private:
        std::filesystem::path vaultPath;
        vaultErr retrieveDefaultVaultPath();
        std::filesystem::path getConfigDirectory();

};