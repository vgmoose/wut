#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <cstdint>

// trim from start
static inline std::string &ltrim(std::string &s) {
   s.erase(s.begin(), std::find_if(s.begin(), s.end(),
      std::not1(std::ptr_fun<int, int>(std::isspace))));
   return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
   s.erase(std::find_if(s.rbegin(), s.rend(),
      std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
   return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
   return ltrim(rtrim(s));
}

uint32_t byte_swap(uint32_t v) {
   return ((v >> 24) & 0xff) |
      ((v << 8) & 0xff0000) |
      ((v >> 8) & 0xff00) |
      ((v << 24) & 0xff000000);
}

enum class ReadMode
{
   INVALID,
   TEXT,
   DATA
};

void writeExports(std::ofstream &out, std::string moduleName, int type, std::vector<std::string> exports)
{
   // Align module name up to 8 bytes
   auto moduleNameSize = (moduleName.length() + 1 + 7) & ~7;

   // Calculate the data block size
   auto exportSecSize = exports.size() * 8;
   if (exportSecSize < moduleNameSize) {
      exportSecSize = moduleNameSize;
   }

   uint32_t exportsHash = 0xBADADADA;

   uint32_t *secData = new uint32_t[exportSecSize / 4];
   memset(secData, 0, exportSecSize);
   memcpy(secData, moduleName.c_str(), moduleName.length());

   out << std::endl;

   out << ".section .fimport_" << moduleName << std::endl;
   out << ".align 0" << std::endl;
   out << std::endl;

   out << ".long " << exports.size() << std::endl;
   out << ".long 0x" << std::hex << exportsHash << std::endl;
   out << std::endl;

   for (auto i = 0; i < exportSecSize / 8; ++i) {
      if (i < exports.size()) {
         out << ".global " << exports[i] << std::endl;
         out << ".type " << exports[i] << ", " << type << std::endl;
         out << exports[i] << ":" << std::endl;
      }

      out << ".long 0x" << std::hex << byte_swap(secData[i * 2 + 0]) << std::endl;
      out << ".long 0x" << std::hex << byte_swap(secData[i * 2 + 1]) << std::endl;
      out << std::endl;
   }

   delete[] secData;
}

int main(int argc, char **argv)
{
   std::string moduleName;
   std::vector<std::string> funcExports, dataExports;
   ReadMode readMode = ReadMode::INVALID;

   if (argc < 3) {
      std::cout << argv[0] << " <exports.def> <output.S>" << std::endl;
      return 0;
   }

   {
      std::ifstream in;
      in.open(argv[1]);

      if (!in.is_open()) {
         std::cout << "Could not open file " << argv[1] << " for reading" << std::endl;
         return -1;
      }

      std::string line;
      while (std::getline(in, line)) {
         // Trim comments
         std::size_t commentOffset = line.find("//");
         if (commentOffset != std::string::npos) {
            line = line.substr(0, commentOffset);
         }

         // Trim whitespace
         line = trim(line);

         // Skip blank lines
         if (line.length() == 0) {
            continue;
         }

         // Look for section headers
         if (line[0] == ':') {
            if (line.substr(1) == "TEXT") {
               readMode = ReadMode::TEXT;
            } else if (line.substr(1) == "DATA") {
               readMode = ReadMode::DATA;
            } else if (line.substr(1, 4) == "NAME") {
               moduleName = line.substr(6);
            } else {
               std::cout << "Unexpected section type" << std::endl;
               return -1;
            }
            continue;
         }

         if (readMode == ReadMode::TEXT) {
            funcExports.push_back(line);
         } else if (readMode == ReadMode::DATA) {
            dataExports.push_back(line);
         } else {
            std::cout << "Unexpected section data" << std::endl;
            return -1;
         }
      }
   }

   {
      std::ofstream out;
      out.open(argv[2]);

      if (!out.is_open()) {
         std::cout << "Could not open file " << argv[2] << " for writing" << std::endl;
         return -1;
      }

      if (funcExports.size() > 0) {
         writeExports(out, moduleName, 2, funcExports);
      }

      if (dataExports.size() > 0) {
         writeExports(out, moduleName, 1, dataExports);
      }
   }

   return 0;
}
