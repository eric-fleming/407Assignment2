/*-------------------------------------------------------------------------*
 *---									---*
 *---		launcherTimer.c						---*
 *---									---*
 *---	    This files defines a silly program that launches another	---*
 *---	program, and gives it 'TIME_LIMIT_NUM_SECONDS' seconds to	---*
 *---	finish.  If it finishes in time it prints "Congratulations!".	---*
 *---	If not, it forces it to finish by sending it SIGINT.		---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1.0		2017 July 26		Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

//
//		Header inclusion:
//

#include	"headers.h"


//
//		Global variable definition:
//

//  PURPOSE:  To hold '1' when the time is up, or '0' before then.
int		isTimeUp		= 0;


//  PURPOSE:  To hold '1' after the child has stopped, or '0' before then.
int		didChildStop		= 0;


//
//		Global function definition:
//

//  PURPOSE:  To handle when the child process has stopped by unzombie-ing it.
//  	  Ignores 'sig'.  No return value.
void		sigChildHandler	(int	sig
				)
{
  //  wait for the child so it does not stay a zombie process
  waitpid(-1,NULL,0);
  if  (isTimeUp){
	  printf("Timer: \"Sorry, time is up!\"\n");
  }
  else{
	  printf("Timer: \"Congratulations!\"\n");
  }
  didChildStop	   = 1;
}


//  PURPOSE:  To run the program.  Ignores parameters.  Sends 'EXIT_SUCCESS'
//	to OS.
int		main		()
{
  //  I.  Application validity check:
  printf("Yo I LAUNCHED\n");
  //  II.  Do program:
  pid_t			childId;

  //  Do something here
  childId = fork();
  if(childId == 0){
	  printf("I FORKED and about to execl ANSWERER\n");
	  execl(ANSWERER_PROG,ANSWERER_PROG,NULL);
  }
  else{
	  int secs;

	  for  (secs = TIME_LIMIT_NUM_SECONDS;  (secs > 0) && !didChildStop;  secs--)
	  {
		  printf("Timer: \"%d seconds\"\n",secs);
		  sleep(1);
	  }

	  isTimeUp	= 1;

	  if  (!didChildStop)
	  {
		  printf("Timer: \"%d seconds\"\n",secs);
		  //  Stop the child process by sending it SIGINT
		  printf("sent SIGINT\n");
		  kill(childId,2);
	  }

	  while  ( !didChildStop )
	  {
		  sleep(1);
	  }
  }
  return(EXIT_SUCCESS);
}
