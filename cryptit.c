#include <crypt.h>
#include <unistd.h>
main()
{
printf("%s\n", crypt("feb15", "aa"));
}
