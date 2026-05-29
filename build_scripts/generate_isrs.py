import sys
import os

# if (len(sys.argv)) !=0:
#   print("Incorrect usage!")
#   print("Usage: generate_isrs.py")
#   sys.exit(1)

script_dir = os.path.dirname(os.path.abspath(__file__))

target_dir = os.path.normpath(os.path.join(script_dir, "..", "src", "kernel", "arch", "i686"))
ISRS_GEN_C = os.path.join(target_dir, "isrs_gen.c")
ISRS_GEN_ASM = os.path.join(target_dir, "isrs_gen.inc")

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
  f.write(";; auto generated!! ;;\n")

  for i in range(256):
      if i in ISRS_WITH_ERR_CODE:
        f.write(f"ISR_ERROR {i}\n")
      else:
        f.write(f"ISR_NOERROR {i}\n")


