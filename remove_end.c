remove_end(char *str, char ch)
{
/*
        char *str=malloc(30);
        strcpy(str, "/user/local/eucalyptus//");
*/
        int len=strlen(str), i=1;

/*      for(i; i<len; i++) {
                if(str[len-i] == ch)
                        str[len-i] = '\0';
                else 
                        break;
        }
*/
        while (str[len-i] == ch) {
                str[len-i] = '\0';
                i++;
        } 
}
