/*
  This tool generates a linker script that defines symbols from
  an ELF file.

  This is used in HeliumOS to link kernel modules with the ability
  to access core kernel symbols.
*/

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <elf.h>

int main(int argc, char **argv)
{
  if(argc != 3)
  {
    fprintf(stderr, "Usage: %s <ELF-file> <ld-output-file>\n", argv[0]);
    return 1;
  }

  FILE *elf = fopen(argv[1], "r");
  if(!elf)
  {
    fprintf(stderr, "Could not open ELF file '%s'.\n", argv[1]);
    return 1;
  }

  FILE *ld = fopen(argv[2], "w");
  if(!ld)
  {
    fprintf(stderr, "Could not open linker script file '%s'.\n", argv[2]);
    return 2;
  }

  // Read symbol file header
  Elf32_Ehdr eh;
  fread(&eh, sizeof(eh), 1, elf);

  size_t sht_size = eh.e_shnum * eh.e_shentsize;
  fseek(elf, eh.e_shoff, SEEK_SET);

  uint8_t sht[sht_size];
  fread(sht, eh.e_shentsize, eh.e_shnum, elf);

  Elf32_Shdr *shstrtab_sh = (void *) sht + eh.e_shstrndx * eh.e_shentsize;

  // Load section header names from ELF file.
  uint8_t shstrtab[shstrtab_sh->sh_size];
  fseek(elf, shstrtab_sh->sh_offset, SEEK_SET);
  fread(shstrtab, 1, sizeof(shstrtab), elf);

  // Find string table
  Elf32_Shdr *strtab_sh = 0;
  for(size_t i = 0; i < eh.e_shnum; ++i)
  {
    Elf32_Shdr *sh = (void *) sht + i * eh.e_shentsize;

    if(sh->sh_type != SHT_STRTAB)
      continue;
    if(strcmp(".strtab", shstrtab + sh->sh_name))
      continue;

    // We found the string table.
    strtab_sh = sh;
    break;
  }

  if(!strtab_sh)
  {
    fprintf(
      stderr,
      "Could not find string table for Elf file '%s'.\n", argv[1]
    );
    return 1;
  }

  // Load string table from ELF file.
  uint8_t strtab[strtab_sh->sh_size];
  fseek(elf, strtab_sh->sh_offset, SEEK_SET);
  fread(strtab, 1, sizeof(strtab), elf);

  // Now find symbol table
  for(size_t i = 0; i < eh.e_shnum; ++i)
  {
    Elf32_Shdr *sh = (void *) sht + i * eh.e_shentsize;

    // Skip this section entry if it is not a symbol table
    if(sh->sh_type != SHT_SYMTAB)
      continue;

    size_t sym_count = sh->sh_size / sh->sh_entsize;

    // Load the symbol table from the ELF file
    uint8_t symtab[sh->sh_size];
    fseek(elf, sh->sh_offset, SEEK_SET);
    fread(symtab, sh->sh_entsize, sym_count, elf);

    // Loop for each symbol and output a line in the linker
    // script for that symbol.
    for(size_t j = 1; j < sym_count; ++j)
    {
      Elf32_Sym *sym = (void *) symtab + j * sh->sh_entsize;
      char *name = strtab + sym->st_name;
      uint32_t val = sym->st_value;

      // If the name is empty skip this symbol.
      if(!strlen(name))
        continue;

      // If the symbol is not global, it should not
      // be outputed to the linker script.
      if(ELF32_ST_BIND(sym->st_info) != STB_GLOBAL)
        continue;

      fprintf(ld, "%s = %#0lx;\n", name, val);
    }
  }
}
