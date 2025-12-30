#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "vault.h"

using namespace std;

//print instructions on how to use
static void PrintUsage() {
    std::cerr
        << "Usage:\n"
        << "  pm init <vault_path>\n"
        << "  pm init <vault_path> -f\n"
        << "  pm defaultPath <vault_path>\n"
        ;
}

static bool IsForceFlag(const std::string& arg) {
    return arg == "-f";
}
static bool validVaultPath(const std::string& VaultPath){
    if (VaultPath.empty() || (!VaultPath.empty() && VaultPath[0] == '-')) {
        return false;
    }
    return true;
}



int main (int argc, char** argv)
{   
    if(argc < 2){
        PrintUsage();
        return 2;
    }

    const std::string cmd = argv[1];
    bool force = false;
    std::string vaultPath;
    vaultErr result;

    if(cmd == "init"){
        // Max args: pm init -f path | min args: pm init path
        if (argc < 3 || argc > 4) {
            PrintUsage();
            return 2; 
        }
         // If there are only 3 args, other arg must be path
        if (argc == 3) {
            vaultPath = argv[2];
            // Reject paths that look like flags (e.g. "pm init --weird")
            if (!validVaultPath(vaultPath)) {
                PrintUsage();
                return 2;
            }
        } else { // argc == 4
            const std::string a2 = argv[2];
            const std::string a3 = argv[3];

            // Figure out which arg is -f(--force)
            if (IsForceFlag(a3) && !IsForceFlag(a2)) {
                force = true;
                vaultPath = a2;
                if(!validVaultPath(vaultPath)){
                    PrintUsage();
                    return 2;
                }
            } else {
                // If flag not correct then exit with usage
                PrintUsage();
                return 2;
            }
        }
        // if all pass, init vault
        vault v(vaultPath);
        result = v.initVault(force);
        if(result == vaultErr::Success){
            cout << "Initialized vault: " << vaultPath << "\n"
            << "Do you want to set this as your default vault? [y/n] ";
            char response;
            cin >> response;
            if(response == 'y'){
                result = v.setDefaultFilepath(vaultPath);
                if(result == vaultErr::Success){
                    cout << "Vault Default Set: " << vaultPath << endl;
                    return 0;
                }
                else{
                    cerr << "Error"<<endl;
                    return 2;
                } 
            }
        }
        else if(result == vaultErr::VaultExists){
            cerr << "Vault Exists, use -f" << endl;
            return 2;
        }
        else{
            cerr << "Error"<<endl;
            return 2;
        }


    }
    else if(cmd == "defaultPath"){
        const std::string newPath = argv[2];
        vault v;
        result = v.setDefaultFilepath(newPath);
                if(result == vaultErr::Success){
            cout << "Vault Default Set: " << newPath << endl;
            return 0;
        }
        else{
            cerr << "Error"<<endl;
            return 2;
        }

    }


    cerr << "Unknown command: " << cmd << endl;
    PrintUsage();
    return 2;
}