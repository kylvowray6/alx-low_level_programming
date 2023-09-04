#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <elf.h>
#include "main.h"

/**
 * print_elf_info - Prints information from the ELF header.
 * @elf_header: A pointer to the ELF header structure.
 */
void print_elf_info(Elf64_Ehdr *elf_header)
{
    printf("ELF Header:\n");
    printf("  Magic:   ");
    for (int i = 0; i < EI_NIDENT; i++)
        printf("%02x ", elf_header->e_ident[i]);
    printf("\n");
    printf("  Class:                             ");
    switch (elf_header->e_ident[EI_CLASS])
    {
        case ELFCLASS32:
            printf("ELF32\n");
            break;
        case ELFCLASS64:
            printf("ELF64\n");
            break;
        default:
            printf("<unknown>\n");
            break;
    }
    printf("  Data:                              ");
    switch (elf_header->e_ident[EI_DATA])
    {
        case ELFDATA2LSB:
            printf("2's complement, little endian\n");
            break;
        case ELFDATA2MSB:
            printf("2's complement, big endian\n");
            break;
        default:
            printf("<unknown>\n");
            break;
    }
    printf("  Version:                           %d (current)\n", elf_header->e_ident[EI_VERSION]);
    printf("  OS/ABI:                            ");
    switch (elf_header->e_ident[EI_OSABI])
    {
        case ELFOSABI_SYSV:
            printf("UNIX - System V\n");
            break;
        case ELFOSABI_NETBSD:
            printf("UNIX - NetBSD\n");
            break;
        case ELFOSABI_SOLARIS:
            printf("UNIX - Solaris\n");
            break;
        default:
            printf("<unknown: %d>\n", elf_header->e_ident[EI_OSABI]);
            break;
    }
    printf("  ABI Version:                       %d\n", elf_header->e_ident[EI_ABIVERSION]);
    printf("  Type:                              ");
    switch (elf_header->e_type)
    {
        case ET_NONE:
            printf("NONE (Unknown)\n");
            break;
        case ET_REL:
            printf("REL (Relocatable file)\n");
            break;
        case ET_EXEC:
            printf("EXEC (Executable file)\n");
            break;
        case ET_DYN:
            printf("DYN (Shared object file)\n");
            break;
        case ET_CORE:
            printf("CORE (Core file)\n");
            break;
        default:
            printf("<unknown: %d>\n", elf_header->e_type);
            break;
    }
    printf("  Entry point address:               %#lx\n", (unsigned long)elf_header->e_entry);
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
        return (EXIT_FAILURE);
    }

    fd = open(av[1], O_RDONLY);
    if (fd == -1)
    {
        dprintf(2, "Error: Can't open file %s\n", av[1]);
        return (EXIT_FAILURE);
    }

    if (read(fd, &elf_header, sizeof(Elf64_Ehdr)) != sizeof(Elf64_Ehdr))
    {
        dprintf(2, "Error: Can't read from file %s\n", av[1]);
        close(fd);
        return (EXIT_FAILURE);
    }

    if (elf_header.e_ident[EI_MAG0] != ELFMAG0 ||
        elf_header.e_ident[EI_MAG1] != ELFMAG1 ||
        elf_header.e_ident[EI_MAG2] != ELFMAG2 ||
        elf_header.e_ident[EI_MAG3] != ELFMAG3)
    {
        dprintf(2, "Error: %s is not an ELF file\n", av[1]);
        close(fd);
        return (EXIT_FAILURE);
    }

    print_elf_info(&elf_header);

    close(fd);
    return (EXIT_SUCCESS);
}
