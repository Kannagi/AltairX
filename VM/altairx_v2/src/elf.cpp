#include <elf.hpp>
#include <fstream>
#include <iostream>

#include <llvm/Object/ELF.h>
#include <llvm/Object/ELFTypes.h>

using ELFFileType = llvm::object::ELFFile<llvm::object::ELF64LE>;

namespace
{

std::optional<AxELFFile> convert_elf(const ELFFileType& elf)
{
  //auto&& header = elf.getHeader();
  auto sections = elf.sections();
  if(auto error = sections.takeError(); error)
  {
    std::cout << "Error: Failed to parse ELF sections:\n"
      << "  " << llvm::toString(std::move(error)).c_str() << std::endl;
    return std::nullopt;
  }

  AxELFFile output;
  output.sections.reserve(sections->size());

  for(auto&& section : *sections)
  {
    auto& current = output.sections.emplace_back();

    if(section.sh_flags & 0x02) // alloc
    {
      auto content = elf.getSectionContents(section);
      if(auto error{content.takeError()}; error)
      {
        std::cerr << "Error: Failed to get ELF section content:\n"
          << "  " << llvm::toString(std::move(error)).c_str() << std::endl;
        return std::nullopt;
      }

      current.content = content->vec();
    }

    current.type = section.sh_type;
    current.flags = section.sh_flags;
    current.addr = section.sh_addr;
    current.offset = section.sh_offset;
    current.size = section.sh_size;
    current.link = section.sh_link;
    current.info = section.sh_info;
    current.addralign = section.sh_addralign;
    current.entsize = section.sh_size;
  }

  return output;
}

}

std::optional<AxELFFile> AxELFFile::from_file(const std::filesystem::path& path)
{
  std::ifstream file{path, std::ios_base::binary};
  if(!file.is_open())
  {
    std::cerr << "Failed to open file \"" << path.generic_string() << "\"" << std::endl;
    return std::nullopt;
  }

  file.seekg(0, std::ios::end);
  std::streampos filesize = file.tellg();
  file.seekg(0, std::ios::beg);
  std::vector<char> data; // must outlive the llvm ELF instrance
  data.resize(filesize);
  file.read(data.data(), data.size());

  auto elf = ELFFileType::create(llvm::StringRef{data.data(), data.size()});
  if(auto error = elf.takeError(); error)
  {
    llvm::consumeError(std::move(error));
    return std::nullopt;
  }

  return convert_elf(*elf);
}
