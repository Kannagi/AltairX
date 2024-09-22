#ifndef AXELF_HPP_INCLUDED
#define AXELF_HPP_INCLUDED

#include <optional>
#include <filesystem>

enum : uint64_t
{
    AX_SHT_NULL = 0x00,          // Section header table entry unused
    AX_SHT_PROGBITS = 0x01,      // Program data
    AX_SHT_SYMTAB = 0x02,        // Symbol table
    AX_SHT_STRTAB = 0x03,        // String table
    AX_SHT_RELA = 0x04,          // Relocation entries with addends
    AX_SHT_HASH = 0x05,          // Symbol hash table
    AX_SHT_DYNAMIC = 0x06,       // Dynamic linking information
    AX_SHT_NOTE = 0x07,          // Notes
    AX_SHT_NOBITS = 0x08,        // Program space with no data(bss)
    AX_SHT_REL = 0x09,           // Relocation entries, no addends
    AX_SHT_SHLIB = 0x0A,         // Reserved
    AX_SHT_DYNSYM = 0x0B,        // Dynamic linker symbol table
    AX_SHT_INIT_ARRAY = 0x0E,    // Array of constructors
    AX_SHT_FINI_ARRAY = 0x0F,    // Array of destructors
    AX_SHT_PREINIT_ARRAY = 0x10, // Array of pre - constructors
    AX_SHT_GROUP = 0x11,         // Section group
    AX_SHT_SYMTAB_SHNDX = 0x12,  // Extended section indices
    AX_SHT_NUM = 0x13,           // Number of defined types.
};

enum : uint64_t
{
    AX_SHF_WRITE = 0x01,             // Writable
    AX_SHF_ALLOC = 0x02,             // Occupies memory during execution
    AX_SHF_EXECINSTR = 0x04,         // Executable
    AX_SHF_MERGE = 0x10,             // Might be merged
    AX_SHF_STRINGS = 0x20,           // Contains null - terminated strings
    AX_SHF_INFO_LINK = 0x40,         //'sh_info' contains SHT index
    AX_SHF_LINK_ORDER = 0x80,        // Preserve order after combining
    AX_SHF_OS_NONCONFORMING = 0x100, // Non - standard OS specific handling required
    AX_SHF_GROUP = 0x200,            // Section is member of a group
    AX_SHF_TLS = 0x400,              // Section hold thread - local data
};

struct AxELFSection
{
    uint32_t type;                // Section type (SHT_*)
    uint64_t flags;               // Section flags (SHF_*)
    uint64_t addr;                // Address where section is to be loaded
    uint64_t offset;              // File offset of section data, in bytes
    uint64_t size;                // Size of section, in bytes
    uint32_t link;                // Section type-specific header table index link
    uint32_t info;                // Section type-specific extra information
    uint64_t addralign;           // Section address alignment
    uint64_t entsize;             // Size of records contained within the section
    std::vector<uint8_t> content; // Copy of the actual content from the file

    bool has_flag(uint64_t flag) const noexcept
    {
        return (flags & flag) != 0;
    }

    bool has_flags(uint64_t other) const noexcept
    {
        return (flags & other) == other;
    }
};

enum : uint32_t
{
    AX_STB_LOCAL = 0,   // Local symbol, not visible outside obj file containing def
    AX_STB_GLOBAL = 1,  // Global symbol, visible to all object files being combined
    AX_STB_WEAK = 2,    // Weak symbol, like global but lower-precedence
    AX_STB_LOOS = 10,   // Lowest operating system-specific binding type
    AX_STB_HIOS = 12,   // Highest operating system-specific binding type
    AX_STB_LOPROC = 13, // Lowest processor-specific binding type
    AX_STB_HIPROC = 15  // Highest processor-specific binding type
};

enum : uint32_t
{
    AX_STT_NOTYPE = 0,  // Symbol's type is not specified
    AX_STT_OBJECT = 1,  // Symbol is a data object (variable, array, etc.)
    AX_STT_FUNC = 2,    // Symbol is executable code (function, etc.)
    AX_STT_SECTION = 3, // Symbol refers to a section
    AX_STT_FILE = 4,    // Local, absolute symbol that refers to a file
    AX_STT_COMMON = 5,  // An uninitialized common block
    AX_STT_TLS = 6,     // Thread local data object
    AX_STT_LOOS = 10,   // Lowest operating system-specific symbol type
    AX_STT_HIOS = 12,   // Highest operating system-specific symbol type
    AX_STT_LOPROC = 13, // Lowest processor-specific symbol type
    AX_STT_HIPROC = 15, // Highest processor-specific symbol type
};

enum : uint32_t
{
    STV_DEFAULT = 0,  // Visibility is specified by binding type
    STV_INTERNAL = 1, // Defined by processor supplements
    STV_HIDDEN = 2,   // Not visible to other components
    STV_PROTECTED = 3 // Visible in other components but not preemptable
};

struct AxELFSymbol
{
    std::string name;    // actual name
    uint64_t st_name;    // index into the object file's symbol string table
    uint64_t value;      // gives the value of the associated symbol. may be an absolute value, an address...
    uint64_t size;       // symbol size. a data object's size is the number of bytes contained in the object
    uint32_t binding;    // symbol binding type, extracted from st_info*
    uint32_t type;       // symbol type, extracted from st_info*
    uint32_t visibility; // currently specifies a symbol's visibility
    uint32_t shndx;      // holds the relevant section header table index.
};

class AxELFFile
{
public:
    static std::optional<AxELFFile> from_file(const std::filesystem::path& path);

    std::vector<AxELFSection> sections;
    std::vector<AxELFSymbol> symbols;
};

#endif
