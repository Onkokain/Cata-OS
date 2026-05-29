import sys

if (len(sys.argv)) <=2:
  print("Incorrect usage!")
  print("Usage: generate_isrs.py <isrs_gen.c> <isrs_gen.inc>")
  sys.exit(1)

ISRS_GEN_C = sys.argv[1]
ISRS_GEN_ASM = sys.argv[2]

ISRS_WITH_ERR_CODE = {8, 10, 11, 12, 13, 14, 17, 21, 29, 30}

# generating C file

with open(ISRS_GEN_C, "w") as f:
  f.write('//Auto generated!!//\n')
  f.write('#include "idt.h" \n')
  f.write('#include "gdt.h" \n\n')

  for i in range(256):
    f.write(f"void __attribute__((cdecl)) i686_ISR{i}();\n")

  f.write("void i686_ISR_InitializeGates()\n")
  f.write("\n{\n")

  for i in range(256):
    f.write(
        f"    i686_IDT_SetGate({i}, "
        f"i686_ISR{i}, "
        f"i686_GDT_CODE_SEGMENT, "
        f"IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT);\n"
    )
  f.write("\n }\n")


# asm file

with open(ISRS_GEN_ASM, "w") as f:
  f.write(";; auto generated!! ;;")
  # f.write("\n [bits 32]")
  # f.write("\n\n extern i686_ISR_Handler\n")

  # f.write(";;;\n")
  # f.write(";;; macros\n")
  # f.write(";;;\n")
  # f.write("%macro ISR_NOERROR 1\n")
  # f.write("global i686_ISR%1\n")
  # f.write("i686_ISR%1:\n")
  # f.write("   push 0 ; dummy error code \n")
  # f.write("   push 0 \n")
  # f.write("   jmp isr_common\n")
  # f.write("%endmacro\n")
  # f.write("\n")

  # f.write("%macro ISR_ERROR 1\n")
  # f.write("global i686_ISR%1\n")
  # f.write("i686_ISR%1:\n")
  # f.write("   push %1 ; error code \n")
  # f.write("   jmp isr_common\n")
  # f.write("%endmacro\n")
  # f.write("\n")

  for i in range(256):
      if i in ISRS_WITH_ERR_CODE:
        f.write(f"ISR_ERRORCODE {i}\n")
      else:
        f.write(f"ISR_NOERRORCODE {i}\n")

  # f.write("isr_common:\n")
  # f.write("push a \n")
  # f.write("xor eax, eax \n")
  # f.write("mov as, ds\n")
  # f.write("push eax\n")
  # f.write("\n")
  # f.write("mov ax, 0x10\n")
  # f.write("mov ds,ax\n")
  # f.write("mov es,ax\n")
  # f.write("mov fs,ax\n")
  # f.write("mov gs,ax\n")
  # f.write("\n")
  # f.write("push esp\n")
  # f.write("call i686_ISR_Handler\n")
  # f.write("add esp, 4 \n")
  # f.write("\n")
  # f.write("pop eax\n")
  # f.write("mov ds,ax\n")
  # f.write("mov es,ax\n")
  # f.write("mov fs,ax\n")
  # f.write("mov gs,ax\n")
  # f.write("popa\n")
  # f.write("add esp, 8\n")
  # f.write("iret\n")


