#include <unistd.h>
#include <stdlib.h>

#define READ_SIZE (256)

int 		xfree(void *ptr)
{
  if (ptr)
    free(ptr);
  return (1);
}

char		*my_strcpy(char *dest, const char *src)
{
  int		len;

  len = -1;
  while (src[++len])
    dest[len] = src[len];
  dest[len] = 0;
  return (dest);
}

char		*xrealloc(char *ptr, size_t size)
{
  char		*new;

  if (size == 0 && ptr)
    return (xfree(ptr), NULL);
  if (ptr == NULL)
    return (((new = malloc(sizeof(char) * (size))) == NULL) ? NULL : new);
  if ((new = malloc(sizeof(char) * (size))) == NULL)
    return (NULL);
  new = my_strcpy(new, ptr);
  return (xfree(ptr), new);
}

static char	    char_from_buffer(const int fd)
{
  static char 	*save = NULL;
  static char 	buf[READ_SIZE];
  static int  	rd = 0;
  static int	i = 0;
  char		to_ret;

  if (rd == 0)
    {
      if ((rd = read(fd, buf, READ_SIZE)) <= 0)
	return (0);
      i = 0;
      save = (char *)buf;
    }
  rd--;
  to_ret = save[i];
  i++;
  return (to_ret);
}

char		*get_next_line(const int fd, size_t len)
{
  char 		*line;
  char		new_char;

  line = NULL;
  new_char = 0;
  if ((new_char = char_from_buffer(fd)) == 0)
    return (NULL);
  if (new_char == '\0')
    return (NULL);
  if ((line = malloc(sizeof(char) * (READ_SIZE + 1))) == NULL)
    return (NULL);
  while (new_char != '\n' && new_char)
    {
      line[len++] = new_char;
      if ((new_char = char_from_buffer(fd)) == 0)
	return (NULL);
      if (len % (READ_SIZE) == 0)
	{
	  line[len] = 0;
	  if ((line = xrealloc(line, (len + READ_SIZE + 1))) == NULL)
	    return (NULL);
	}
    }
  line[len] = 0;
  return (line);
}

#include <stdio.h>
int	main()
{
  char	*tmp;

  tmp = NULL;
  while (xfree(tmp) && (tmp = get_next_line(0, 0)))
    printf("%s\n", tmp);
  return (0);
}
