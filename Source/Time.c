#include "../Header/Core.h"


struct time_char{
    char *weekday;
    char *mouth;
    char *day;
    char *hour;
    char *minute;
    char *second;
    char *year;
};

typedef struct time_char time_t_char;

char *get_modification_time(struct stat data){
    time_t mod_time = data.st_mtime;
    char *time = ctime(&mod_time);
    char **splited = ft_split(time, ' ');
    time_t_char d;
    d.weekday = splited[0];
    d.mouth = splited[1];
    d.day = splited[2];
    char **hourly = ft_split(splited[3], ':');
    d.hour = hourly[0];
    d.minute = hourly[1];
    d.second = hourly[2];
    d.year = splited[4];
    free(splited);
    free(hourly);
    return ft_strjoin_va(8, d.mouth, " ", d.day, " ", d.hour, ":", d.minute, " ", d.year);
}