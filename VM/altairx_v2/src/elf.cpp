#include <elf.hpp>
#include <fstream>
#include <iostream>

#include <llvm/Object/ELF.h>
#include <llvm/Object/ELFTypes.h>

using ELFFileType = llvm::object::ELFFile<llvm::object::ELF64LE>;

namespace
{

std::optional<std::vector<AxELFSymbol>> convert_symbols(const ELFFileType& elf, const ELFFileType::Elf_Shdr& section)
{
    auto symtab = elf.getStringTableForSymtab(section);
    if(auto error{symtab.takeError()}; error)
    {
        std::cerr << "Error: Failed to get ELF symtab section string table:\n"
                  << "  " << llvm::toString(std::move(error)).c_str() << std::endl;
        return std::nullopt;
    }

    auto symbols = elf.symbols(&section);
    if(auto error{symbols.takeError()}; error)
    {
        std::cerr << "Error: Failed to get ELF symbol:\n"
                  << "  " << llvm::toString(std::move(error)).c_str() << std::endl;
        return std::nullopt;
    }

    std::vector<AxELFSymbol> output;
    output.reserve(symbols->size());

    for(auto&& symbol : *symbols)
    {
        auto name = symbol.getName(*symtab);
        if(auto error{name.takeError()}; error)
        {
            std::cerr << "Warning: Failed to get ELF symbol name:\n"
                      << "  " << llvm::toString(std::move(error)).c_str() << std::endl;
        }

        auto& current = output.emplace_back();
        current.name = name->str();
        current.st_name = symbol.st_name;
        current.value = symbol.st_value;
        current.size = symbol.st_size;
        current.binding = symbol.getBinding();
        current.type = symbol.getType();
        current.visibility = symbol.st_other & 0x03u;
        current.shndx = symbol.st_shndx;
    }

    return output;
}

std::optional<AxELFFile> convert_elf(const ELFFileType& elf)
{
    // auto&& header = elf.getHeader();
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
        if(section.sh_type == AX_SHT_SYMTAB) // preprocess symbol table for later uses!
        {
            auto symbols = convert_symbols(elf, section);
            if(symbols)
            {
                output.symbols = *std::move(symbols);
            }
        }

        // store raw section info
        auto& current = output.sections.emplace_back();
        current.type = section.sh_type;
        current.flags = section.sh_flags;
        current.addr = section.sh_addr;
        current.offset = section.sh_offset;
        current.size = section.sh_size;
        current.link = section.sh_link;
        current.info = section.sh_info;
        current.addralign = section.sh_addralign;
        current.entsize = section.sh_size;

        // get content of allocatable sections
        if((section.sh_flags & AX_SHF_ALLOC) != 0)
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
