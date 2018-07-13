#include <unistd.h>
#include <stdlib.h>

const size_t READ_SIZE(0x200)

bool my_free(void *ptr)
{
	if (ptr)
		free(ptr);
	return (true);
}

void *my_memset(void *s, const int c, const size_t n)
{
	char *d = (char *)s;
	size_t i = n;

	if (i) {
		i++;
		while (--i) {
			*d++ = (unsigned char)c;
		}
	}
        return (s);
}

void *my_memcpy(void *dest, const void *src, const size_t n)
{
	char *dp = dest;
        const char *sp = src;
        size_t i = n;

	if (i) {
		i++;
		while (--i) {
			*dp++ = *sp++;
		}
	}
        return (dest);
}

void *my_realloc(void *ptr, const size_t size)
{
	void *new_mem = NULL;

	if (!size) {
		my_free(ptr);
		return (NULL);
	}
	new_mem = malloc(size);
	if (!new_mem) {
		return (NULL);
	}
	if (!ptr) {
		return (new_mem);
	}
	my_memcpy(new_mem, ptr, size);
	my_free(ptr);
	return (new_mem);
}

char *get_next_line(const int fd)
{
	static char buf[READ_SIZE];
	static ssize_t rd = 0;
	static ssize_t s_rd = 0;
	char *s = NULL;
	char nc = 0;
	size_t i = 0;

	if (!(s = malloc(sizeof(char) * (READ_SIZE + 1))))
		return (NULL);
	while (nc != '\n') {
		if ((!rd && my_memset(buf, 0, READ_SIZE) &&
		(s_rd = (rd = read(fd, buf, READ_SIZE))) && s_rd <= 0) ||
		(((nc = buf[((rd--) - s_rd) * -1]) != '\n' &&
		!(s[i++] = nc)) || (!(i % READ_SIZE) && !(s[i] = 0) &&
		!(s = my_realloc(s, sizeof(char) * (i + READ_SIZE + 1))))))
			return (NULL);
	}
	s[i] = 0;
	return (s);
}

#include <stdio.h>
int main()
{
	char *str = NULL;
	int file_desc = 0;

	while ((str = get_next_line(file_desc)) {
		printf("%s\n", str);
		free(str);
	}
	return (0);
}
