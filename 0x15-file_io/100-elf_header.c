#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <elf.h>
#include "main.h"

/**
 * print_elf_info - Prints information from the ELF header.
 * @elf_header: Pointer to the ELF header structure.
 */
void print_elf_info(Elf64_Ehdr *elf_header)
{
    printf("ELF Header:\n");
    printf("  Magic:   ");
    for (int i = 0; i < EI_NIDENT; i++)
        printf("%02x ", elf_header->e_ident[i]);
    printf("\n");
    printf("  Class:                             %s\n", elf_header->e_ident[EI_CLASS] == ELFCLASS32 ? "ELF32" : "ELF64");
    printf("  Data:                              %s\n", elf_header->e_ident[EI_DATA] == ELFDATA2LSB ? "2's complement, little endian" : "2's complement, big endian");
    printf("  Version:                           %d (current)\n", elf_header->e_ident[EI_VERSION]);
    printf("  OS/ABI:                            %s\n", elf_header->e_ident[EI_OSABI] == ELFOSABI_SYSV ? "UNIX - System V" :
                                                        elf_header->e_ident[EI_OSABI] == ELFOSABI_NETBSD ? "UNIX - NetBSD" :
                                                        elf_header->e_ident[EI_OSABI] == ELFOSABI_SOLARIS ? "UNIX - Solaris" :
                                                        elf_header->e_ident[EI_OSABI] == ELFOSABI_SORTIX ? "<unknown: 53>" : "unknown");
    printf("  ABI Version:                       %d\n", elf_header->e_ident[EI_ABIVERSION]);
    printf("  Type:                              ");
    switch (elf_header->e_type)
    {
        case ET_NONE: printf("NONE (Unknown type)\n"); break;
        case ET_REL: printf("REL (Relocatable file)\n"); break;
        case ET_EXEC: printf("EXEC (Executable file)\n"); break;
        case ET_DYN: printf("DYN (Shared object file)\n"); break;
        case ET_CORE: printf("CORE (Core file)\n"); break;
        default: printf("<unknown>\n"); break;
    }
    printf("  Entry point address:               0x%lx\n", (unsigned long)elf_header->e_entry);
}

/**
 * main - Entry point of the program.
 * @ac: The number of command-line arguments.
 * @av: An array of command-line argument strings.
 *
 * Return: 0 on success, or the corresponding exit code on failure.
 */
int main(int ac, char *av[])
{
    int fd;
    Elf64_Ehdr elf_header;

    if (ac != 2)
    {
        dprintf(2, "Usage: %s elf_filename\n", av[0]);
        return (98);
    }

    fd = open(av[1], O_RDONLY);
    if (fd == -1)
    {
        dprintf(2, "Error: Can't open file %s\n", av[1]);
        return (98);
    }

    if (read(fd, &elf_header, sizeof(Elf64_Ehdr)) != sizeof(Elf64_Ehdr))
    {
        dprintf(2, "Error: Can't read from file %s\n", av[1]);
        close(fd);
        return (98);
    }

    if (elf_header.e_ident[EI_MAG0] != ELFMAG0 ||
        elf_header.e_ident[EI_MAG1] != ELFMAG1 ||
        elf_header.e_ident[EI_MAG2] != ELFMAG2 ||
        elf_header.e_ident[EI_MAG3] != ELFMAG3)
    {
        dprintf(2, "Error: Not an ELF file: %s\n", av[1]);
        close(fd);
        return (98);
    }

    print_elf_info(&elf_header);
    close(fd);
    return (0);
}
