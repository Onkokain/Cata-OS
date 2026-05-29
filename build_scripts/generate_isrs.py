import sys

# if (len(sys.argv)) <=2:
#   print("Incorrect usage!")
#   print("Usage: generate_isrs.py <isrs_gen.c> <isrs_gen.inc>")
#   sys.exit(1)

ISRS_GEN_C = "../src/kernel/arch/i686/isrs_gen.c"
ISRS_GEN_ASM = "../src/kernel/arch/i686/isrs_gen.inc"

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


  for i in range(256):
      if i in ISRS_WITH_ERR_CODE:
        f.write(f"ISR_ERRORCODE {i}\n")
      else:
        f.write(f"ISR_NOERRORCODE {i}\n")


