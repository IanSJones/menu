void str_upper (s)
char		*s;
{
   while (*s)
   {
      if (*s >= 'a'  &&  *s <= 'z')
         *s = (*s + 'A' - 'a');
      s++;
   }
}
