#include <string>;
#include <iostream>;
#include <fstream>;
#include <filesystem>;

using namespace std;

//print instructions on how to use
static void PrintUsage() {
    std::cerr
        << "Usage:\n"
        << "  pm init <vault_path>\n"
        << "\n"
        << "Example:\n"
        << "  pm init ./myvault.dat\n";
}

static bool IsForceFlag(const std::string& arg) {
    return arg == "-f" || arg == "--force";
}

static int CmdInit(const std::filesystem::path& vaultPath, const bool& force) {

    //create the parent folder if it doesn't exist yet
    const auto parent = vaultPath.parent_path();
    if (!parent.empty()) {
        std::filesystem::create_directories(parent);
    }

    if(exists(vaultPath) && !force ){
        std::cerr << "Valut already exists use --force"<<endl;
        return 2;
    }
    std::ofstream out(vaultPath, std::ios::binary | std::ios::trunc);
    if (!out) {
        std::cerr << "Error: could not create vault file: " << vaultPath.string() << "\n";
        return 1;
    }

    // Minimal “file signature” so you can detect it later.
    const std::string magic = "PMVAULT\0"; // includes a NUL byte
    const std::string version = "v1\n";
    out.write(magic.data(), static_cast<std::streamsize>(magic.size()));
    out.write(version.data(), static_cast<std::streamsize>(version.size()));

    if (!out) {
        std::cerr << "Error: failed while writing vault file: " << vaultPath.string() << "\n";
        return 1;
    }

    std::cout << "Initialized vault: " << vaultPath.string() << "\n";
    return 0;
}

int main (int argc, char** argv)
{   
    if(argc < 2){
        PrintUsage();
        return 2;
    }

    const std::string cmd = argv[1];
    bool force = false;
    std::string VaultPath;

// Max args: pm init -f path  (argc == 4)
    if (argc < 3 || argc > 4) {
        PrintUsage();
        return 2;
    }

    // If there are only 3 args, other arg must be path
    if (argc == 3) {
        VaultPath = argv[2];
        // Reject paths that look like flags (e.g. "pm init --weird")
        if (VaultPath.empty() || (!VaultPath.empty() && VaultPath[0] == '-')) {
            PrintUsage();
            return 2;
        }
    } else { // argc == 4
        const std::string a2 = argv[2];
        const std::string a3 = argv[3];

        // Figure out which arg is -f(--force)
        if (IsForceFlag(a2) && !IsForceFlag(a3)) {
            force = true;
            VaultPath = a3;
        } else if (IsForceFlag(a3) && !IsForceFlag(a2)) {
            force = true;
            VaultPath = a2;
        } else {
            // If neither is -f/--force, or both are flags, then exit with usage
            PrintUsage();
            return 2;
        }
    }



    // Initialize vault
    return CmdInit(std::filesystem::path(VaultPath), force);

    std::cerr << "Unknown command: " << cmd << endl;
    PrintUsage();
    return 2;
}