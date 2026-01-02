#pragma once

#include <filesystem>
#include <fstream>
#include <vector>

enum class vaultErr : int{
    Success = 0,
    NoPath = 1,
    VaultExists = 2,
    VaultCreateFailed = 3,
    VaultWriteFailed = 4,
    VaultReadFailed = 5,
    NoEntry = 6
};

class vault{
    public:
        vault();
        vaultErr setDefaultFilepath(const std::filesystem::path& newDefaultPath);
        vaultErr initVault(const bool& force);
        vaultErr addNewKey(const std::filesystem::path& newName, const std::filesystem::path& newUsername, const std::filesystem::path& newPassword);
        vaultErr listNames(std::vector<std::string>& names);
        vaultErr getKey(const std::string& keyName, std::string* keyValues);

    private:
        std::filesystem::path vaultPath;
        vaultErr retrieveDefaultVaultPath();
        std::filesystem::path getConfigDirectory();

};