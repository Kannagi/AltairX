#include <cstdio>
#include <cstdlib>
#include <vector>

#include <altairx.hpp>
#include <core.hpp>

void AxCore::execute_syscall()
{
    const auto regA = m_regs.gpi[1] & 0xF0;
    const auto regB = m_regs.gpi[1] & 0x0F;
    const auto reg1 = m_regs.gpi[2];

    // printf("syscall : %.2lx %.2lx\n",core->ireg[4],reg1);
    // printf("syscall : %f\n",core->vreg[3*4]);

    if(regA == 0x00) // Kernel 1
    {
        switch(regB)
        {
        case 0:
            printf("AltairX K1 , VM Machine \n");
            break;

        case 1:
            puts(reinterpret_cast<const char*>(m_memory->map(*this, reg1)));
            break;

        case 2:
            m_regs.gpi[1] = 0; // X
            m_regs.gpi[2] = 0; // Y
            break;

        case 3:
            exit(reg1);
            break;

        case 0xE: // getchar
            m_regs.gpi[1] = getchar();
            break;

        default:
            break;
        }
    }
    else if(regA == 0x10) // Kernel 2
    {
    }
    else if(regA == 0x20) // Sound
    {
    }
    else if(regA == 0x30) // GUI
    {
    }
    else if(regA == 0x40) // Net
    {
    }

    m_syscall = 0;
}
