#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "main.h"

/**
 * append_text_to_file - Appends text to the end of a file.
 * @filename: The name of the file to which to append text.
 * @text_content: The NULL-terminated string to append.
 *
 * Return: 1 on success, -1 on failure.
 */
int append_text_to_file(const char *filename, char *text_content)
{
    int fd, write_result;
    ssize_t len = 0;

    if (filename == NULL)
        return (-1);

    if (text_content != NULL)
    {
        while (text_content[len])
            len++;
    }

    fd = open(filename, O_WRONLY | O_APPEND);
    if (fd == -1)
        return (-1);

    if (text_content != NULL)
    {
        write_result = write(fd, text_content, len);
        if (write_result == -1)
        {
            close(fd);
            return (-1);
        }
    }

    close(fd);
    return (1);
}
