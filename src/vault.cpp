#include <filesystem>
#include <iostream>
#include <fstream>
#include "vault.h"



        //default constructor will retrieve vault filepath
        vault::vault(){
            retrieveDefaultVaultPath();
        }
        //constructor runs when a filepath is passed in and set it manually
        vault::vault(const std::filesystem::path& newPath){
            vaultPath = newPath;
        }

        std::filesystem::path vault::getConfigDirectory(){
             std::filesystem::path configPath;
            //get directory for config file
            const char* xdg_config_home = std::getenv("XDG_CONFIG_HOME");
            if(xdg_config_home != nullptr && std::string(xdg_config_home).length()>0){
                configPath = xdg_config_home;
            }
            
            else{ //if that config home path failes, set the config path to home/passwordManager
                const char* home_dir = std::getenv("HOME");
                if(home_dir != nullptr && std::string(home_dir).length()>0){
                    configPath = home_dir;
                }
                else{
                    configPath = std::filesystem::current_path();
                }
            }
            //set config location for file
            configPath /= std::filesystem::path(".config") / "vault.config";
            return configPath;

        }

        vaultErr vault::setDefaultFilepath(const std::filesystem::path& newDefaultPath){
            std::filesystem::path configPath = getConfigDirectory();

            //create parent folder if it doesn't exist
            const auto parent = vaultPath.parent_path();
            if (!parent.empty()) {
                std::filesystem::create_directories(parent);
            }
            //write to file location
            std::ofstream ConfigFile (configPath);

            ConfigFile << newDefaultPath.string();

            ConfigFile.close();
            return vaultErr::Success;

        }

        vaultErr vault::initVault(const bool& force) {

            //create the parent folder if it doesn't exist yet
            const auto parent = vaultPath.parent_path();
            if (!parent.empty()) {
                std::filesystem::create_directories(parent);
            }

            if(exists(vaultPath) && !force ){
                return vaultErr::VaultExists;
            }
            std::ofstream out(vaultPath, std::ios::binary | std::ios::trunc);
            if (!out) {
                return vaultErr::VaultCreateFailed;
            }

            // Minimal “file signature” so you can detect it later.
            const std::string magic = "PMVAULT\0"; // includes a NUL byte
            const std::string version = "v1\n";
            out.write(magic.data(), static_cast<std::streamsize>(magic.size()));
            out.write(version.data(), static_cast<std::streamsize>(version.size()));

            if (!out) {
                return vaultErr::VaultWriteFailed;
            }

            return vaultErr::Success;
        }

        vaultErr vault::retrieveDefaultVaultPath(){
            //get config path            
            std::filesystem::path configFilePath = getConfigDirectory();
            
            //read from file there
            std::ifstream configFile(configFilePath);
            if (!configFile) {
                return vaultErr::VaultReadFailed; // Failure to open the file
            }

            std::string pathStr;
            std::getline(configFile, pathStr);  // Read the first line

            // If pathStr is not empty, convert it to std::filesystem::path and assign it
            if (!pathStr.empty()) {
                vaultPath = pathStr;
                return vaultErr::Success;  // Successfully read the vault path
            }

            //return output
            return vaultErr::VaultReadFailed;
        }
