#include <unistd.h>
#include <stdlib.h>

int 		xfree(void *ptr)
{
  if (ptr)
    free(ptr);
  return (1);
}

char		  *xrealloc(char *ptr, size_t size)
{
  char		*new;

  if (size == 0 && ptr)
    return (xfree(ptr), NULL);
  if (ptr == NULL)
    {
      if ((new = malloc(sizeof(char) * (size))) == NULL)
	return (NULL);
      return (new);
    }
  if ((new = malloc(sizeof(char) * (size))) == NULL)
    return (NULL);
  new = strcpy(new, ptr);
  return (xfree(ptr), new);
}

static char	    char_from_buffer(const int fd)
{
  static char 	*save = NULL;
  static char 	buf[FILE_BUFFER];
  static int  	rd = 0;
  static int	  i = 0;

  if (rd == 0)
    {
      if ((rd = read(fd, buf, FILE_BUFFER)) <= 0)
	return (0);
      i = 0;
      save = (char *)buf;
    }
  rd--;
  return (save[i++]);
}

char		*get_next_line(const int fd, size_t len)
{
  char 	*line;
  char	new_char;

  line = NULL;
  new_char = 0;
  if ((new_char = char_from_buffer(fd)) == 0)
    return (NULL);
  if (new_char == '\0')
    return (NULL);
  if ((line = malloc(sizeof(char) * (FILE_BUFFER + 1))) == NULL)
    return (NULL);
  while (new_char != '\n' && new_char)
    {
      line[len++] = new_char;
      if ((new_char = char_from_buffer(fd)) == 0)
	return (NULL);
      if (len % (FILE_BUFFER + 1) == 0)
	{
	  line[len] = 0;
	  if ((line = xrealloc(line, (len + FILE_BUFFER + 1))) == NULL)
	    return (NULL);
	}
    }
  line[len] = 0;
  return (line);
}
