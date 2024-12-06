#include <string.h>
#include <stdio.h>
#include <locale.h>
#include "WinRarConfig.hpp"
#include "WinRarKeygen.hpp"
#include <system_error>
#include <sstream>
#include <cstring>

void Help() {
    puts("Usage:");
    puts("        ./winrar-keygen <your name> <license type>");
    puts("");
    puts("Example:");
    puts("");
    puts("        ./winrar-keygen \"Github\" \"Github.com\"");
    puts("  or:");
    puts("        ./winrar-keygen \"Github\" \"Github.com\" > rarreg.key");
    puts("  or:");
    puts("        ./winrar-keygen \"Github\" \"Github.com\" | Out-File -Encoding ASCII rarreg.key\n");
}

void PrintRegisterInfo(const WinRarKeygen<WinRarConfig>::RegisterInfo& Info) {
    printf("%s\n", "RAR registration data");
    printf("%s\n", Info.UserName.c_str());
    printf("%s\n", Info.LicenseType.c_str());
    printf("UID=%s\n", Info.UID.c_str());
    for (size_t i = 0; i < Info.HexData.length(); i += 54) {
        printf("%.54s\n", Info.HexData.c_str() + i);
    }
}

int main(int argc, char* argv[]) {
    std::cout << "Program start" << std::endl;
    setlocale(LC_ALL, "");
    if (argc == 3) {
        try {
            PrintRegisterInfo(
                // WinRarKeygen<WinRarConfig>::GenerateRegisterInfo(ToACP(argv[1]).c_str(), ToACP(argv[2]).c_str())
                WinRarKeygen<WinRarConfig>::GenerateRegisterInfo(argv[1], argv[2])
            );
        } catch (std::exception& e) {
            printf("%s\n", e.what());
            return -1;
        }
    } else {
        Help();
    }
    return 0;
}


extern "C"
{
    const char* GetKey(const char* user, const char* license)
    {
        auto RegInfo = WinRarKeygen<WinRarConfig>::GenerateRegisterInfo(user, license);
        std::ostringstream oss;
        oss << "RAR registration data\n";
        oss << RegInfo.UserName << "\n";
        oss << RegInfo.LicenseType << "\n";
        oss << "UID=" << RegInfo.UID << "\n";
        for (size_t i = 0; i < RegInfo.HexData.length(); i += 54)
        {
            oss << RegInfo.HexData.substr(i, 54) << "\n";
        }
        std::string result = oss.str();
        char* cstr = new char[result.length() + 1];
        std::strcpy(cstr, result.c_str());
        return cstr;
    }
}
