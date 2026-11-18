#ifndef CTR_AWARDS_H
#define CTR_AWARDS_H

/* This fork's own achievements (fr_awards.c). */

#include <stdbool.h>

/* Once a frame: checks the save's progress while a game is on. */
void CtrAwards_Frame(bool inGame);
/* An award won and not yet announced: its index, with its name; or for a
 * save's earlier progress won all at once, name NULL and `also` the number.
 * -1 when there is none. */
int CtrAwards_TakeBanner(const char **name, int *also);
unsigned CtrAwards_Count(void);
unsigned CtrAwards_WonCount(void);
bool CtrAwards_Won(unsigned index);
const char *CtrAwards_Name(unsigned index);
const char *CtrAwards_Text(unsigned index);

#endif // CTR_AWARDS_H
