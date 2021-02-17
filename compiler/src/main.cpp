#include <iostream>

#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <IR file>\n";
        return 1;
    }

    llvm::LLVMContext context;
    llvm::SMDiagnostic err;
    std::unique_ptr<llvm::Module> mod = llvm::parseIRFile(argv[1], err, context);

    if(!mod)
    {
        std::cerr << "RIP\n";
        return 1;
    }
}
