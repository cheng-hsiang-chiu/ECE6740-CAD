
/*---------------------------------------------------------------------------*/
/* Definition of internal functions                                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Definition of static functions                                            */
/*---------------------------------------------------------------------------*/


/**Function********************************************************************

  Synopsis    [Allocates the option structure and initializes it.]

  Description []

  SideEffects [none]

  SeeAlso     [ntrReadOptions]

******************************************************************************/
static NtrOptions *
mainInit(
   )
{
    NtrOptions	*option;

    /* Initialize option structure. */
    option = ALLOC(NtrOptions,1);
    option->initialTime    = util_cpu_time();
    option->verify         = FALSE;
    option->second         = FALSE;
    option->file1          = NULL;
    option->file2          = NULL;
    option->traverse       = FALSE;
    option->depend         = FALSE;
    option->image          = NTR_IMAGE_MONO;
    option->imageClip      = 1.0;
    option->approx         = NTR_UNDER_APPROX;
    option->threshold      = -1;
    option->from	   = NTR_FROM_NEW;
    option->groupnsps      = NTR_GROUP_NONE;
    option->closure        = FALSE;
    option->closureClip    = 1.0;
    option->envelope       = FALSE;
    option->scc            = FALSE;
    option->maxflow        = FALSE;
    option->zddtest        = FALSE;
    option->sinkfile       = NULL;
    option->partition      = FALSE;
    option->char2vect      = FALSE;
    option->density        = FALSE;
    option->quality        = 1.0;
    option->decomp         = FALSE;
    option->cofest         = FALSE;
    option->clip           = -1.0;
    option->noBuild        = FALSE;
    option->stateOnly      = FALSE;
    option->node           = NULL;
    option->locGlob        = BNET_GLOBAL_DD;
    option->progress       = FALSE;
    option->cacheSize      = 32768;
    option->maxMemory      = 0;	/* set automatically */
    option->slots          = CUDD_UNIQUE_SLOTS;
    option->ordering       = PI_PS_FROM_FILE;
    option->orderPiPs      = NULL;
    option->reordering     = CUDD_REORDER_NONE;
    option->autoMethod     = CUDD_REORDER_SIFT;
    option->autoDyn        = 0;
    option->treefile       = NULL;
    option->firstReorder   = DD_FIRST_REORDER;
    option->countDead      = FALSE;
    option->maxGrowth      = 20;
    option->groupcheck     = CUDD_GROUP_CHECK7;
    option->arcviolation   = 10;
    option->symmviolation  = 10;
    option->recomb         = DD_DEFAULT_RECOMB;
    option->nodrop         = TRUE;
    option->signatures     = FALSE;
    option->verb           = 0;
    option->gaOnOff        = 0;
    option->populationSize = 0;	/* use default */
    option->numberXovers   = 0;	/* use default */
    option->bdddump	   = FALSE;
    option->dumpFmt	   = 0;	/* dot */
    option->dumpfile	   = NULL;
    option->store          = -1; /* do not store */
    option->storefile      = NULL;
    option->load           = FALSE;
    option->loadfile       = NULL;

    return(option);

} /* end of mainInit */


/**Function********************************************************************

  Synopsis    [Reads the command line options.]

  Description [Reads the command line options. Scans the command line
  one argument at a time and performs a switch on each arguement it
  hits.  Some arguemnts also read in the following arg from the list
  (i.e., -f also gets the filename which should folow.)
  Gives a usage message and exits if any unrecognized args are found.]

  SideEffects [May initialize the random number generator.]

  SeeAlso     [mainInit ntrReadOptionsFile]

******************************************************************************/
static void
ntrReadOptions(
  int  argc,
  char ** argv,
  NtrOptions * option)
{
    int	i = 0;

    if (argc < 2) goto usage;

    if (STRING_EQUAL(argv[1],"-f")) {
	ntrReadOptionsFile(argv[2],&argv,&argc);
    }

    for (i = 1; i < argc; i++) {
	if (argv[i][0] != '-' ) {
	    if (option->file1 == NULL) {
		option->file1 = util_strsav(argv[i]);
	    } else {
		goto usage;
	    }
	} else if (STRING_EQUAL(argv[i],"-second")) {
	    i++;
	    option->file2 = util_strsav(argv[i]);
	    option->second = TRUE;
	} else if (STRING_EQUAL(argv[i],"-verify")) {
	    i++;
	    option->file2 = util_strsav(argv[i]);
	    option->verify = TRUE;
	} else if (STRING_EQUAL(argv[i],"-trav")) {
	    option->traverse = TRUE;
	} else if (STRING_EQUAL(argv[i],"-depend")) {
	    option->traverse = TRUE;
	    option->depend = TRUE;
	} else if (STRING_EQUAL(argv[i],"-image")) {
	    i++;
	    if (STRING_EQUAL(argv[i],"part")) {
		option->image = NTR_IMAGE_PART;
	    } else if (STRING_EQUAL(argv[i],"clip")) {
		option->image = NTR_IMAGE_CLIP;
	    } else if (STRING_EQUAL(argv[i],"depend")) {
		option->image = NTR_IMAGE_DEPEND;
	    } else if (STRING_EQUAL(argv[i],"mono")) {
		option->image = NTR_IMAGE_MONO;
	    } else {
		goto usage;
	    }
	} else if (STRING_EQUAL(argv[i],"-depth")) {
	    i++;
	    option->imageClip = (double) atof(argv[i]);
	} else if (STRING_EQUAL(argv[i],"-cdepth")) {
	    i++;
	    option->closureClip = (double) atof(argv[i]);
	} else if (STRING_EQUAL(argv[i],"-approx")) {
	    i++;
	    if (STRING_EQUAL(argv[i],"under")) {
		option->approx = NTR_UNDER_APPROX;
	    } else if (STRING_EQUAL(argv[i],"over")) {
		option->approx = NTR_OVER_APPROX;
	    } else {
		goto usage;
	    }
	} else if (STRING_EQUAL(argv[i],"-threshold")) {
	    i++;
	    option->threshold = (int) atoi(argv[i]);
	} else if (STRING_EQUAL(argv[i],"-from")) {
	    i++;
	    if (STRING_EQUAL(argv[i],"new")) {
		option->from = NTR_FROM_NEW;
	    } else if (STRING_EQUAL(argv[i],"reached")) {
		option->from = NTR_FROM_REACHED;
	    } else if (STRING_EQUAL(argv[i],"restrict")) {
		option->from = NTR_FROM_RESTRICT;
	    } else if (STRING_EQUAL(argv[i],"compact")) {
		option->from = NTR_FROM_COMPACT;
	    } else if (STRING_EQUAL(argv[i],"squeeze")) {
		option->from = NTR_FROM_SQUEEZE;
	    } else if (STRING_EQUAL(argv[i],"subset")) {
		option->from = NTR_FROM_UNDERAPPROX;
	    } else if (STRING_EQUAL(argv[i],"superset")) {
		option->from = NTR_FROM_OVERAPPROX;
	    } else {
		goto usage;
	    }
	} else if (STRING_EQUAL(argv[i],"-groupnsps")) {
	    i++;
	    if (STRING_EQUAL(argv[i],"none")) {
		option->groupnsps = NTR_GROUP_NONE;
	    } else if (STRING_EQUAL(argv[i],"default")) {
		option->groupnsps = NTR_GROUP_DEFAULT;
	    } else if (STRING_EQUAL(argv[i],"fixed")) {
		option->groupnsps = NTR_GROUP_FIXED;
	    } else {
		goto usage;
	    }
	} else if (STRING_EQUAL(argv[i],"-closure")) {
	    option->closure = TRUE;
	} else if (STRING_EQUAL(argv[i],"-envelope")) {
	    option->envelope = TRUE;
	} else if (STRING_EQUAL(argv[i],"-scc")) {
	    option->scc = TRUE;
	} else if (STRING_EQUAL(argv[i],"-maxflow")) {
	    option->maxflow = TRUE;
	} else if (STRING_EQUAL(argv[i],"-zdd")) {
	    option->zddtest = TRUE;
	} else if (STRING_EQUAL(argv[i],"-sink")) {
	    i++;
	    option->maxflow = TRUE;
	    option->sinkfile = util_strsav(argv[i]);
	} else if (STRING_EQUAL(argv[i],"-part")) {
	    option->partition = TRUE;
	} else if (STRING_EQUAL(argv[i],"-char2vect")) {
	    option->char2vect = TRUE;
	} else if (STRING_EQUAL(argv[i],"-density")) {
	    option->density = TRUE;
	} else if (STRING_EQUAL(argv[i],"-quality")) {
	    i++;
	    option->quality = (double) atof(argv[i]);
	} else if (STRING_EQUAL(argv[i],"-decomp")) {
	    option->decomp = TRUE;
	} else if (STRING_EQUAL(argv[i],"-cofest")) {
	    option->cofest = TRUE;
	} else if (STRING_EQUAL(argv[i],"-clip")) {
	    i++;
	    option->clip = (double) atof(argv[i]);
	    i++;
	    option->file2 = util_strsav(argv[i]);
	} else if (STRING_EQUAL(argv[i],"-nobuild")) {
	    option->noBuild = TRUE;
	    option->reordering = CUDD_REORDER_NONE;
	} else if (STRING_EQUAL(argv[i],"-delta")) {
	    option->stateOnly = TRUE;
	} else if (STRING_EQUAL(argv[i],"-node")) {
	    i++;
	    option->node = util_strsav(argv[i]);
	} else if (STRING_EQUAL(argv[i],"-local")) {
	    option->locGlob = BNET_LOCAL_DD;
	} else if (STRING_EQUAL(argv[i],"-progress")) {
	    option->progress = TRUE;
	} else if (STRING_EQUAL(argv[i],"-cache")) {
	    i++;
	    option->cacheSize = (int) atoi(argv[i]);
	} else if (STRING_EQUAL(argv[i],"-maxmem")) {
	    i++;
	    option->maxMemory = 1048576 * (int) atoi(argv[i]);
	} else if (STRING_EQUAL(argv[i],"-slots")) {
	    i++;
	    option->slots = (int) atoi(argv[i]);
	} else if (STRING_EQUAL(argv[i],"-ordering")) {
	    i++;
	    if (STRING_EQUAL(argv[i],"dfs")) {
		option->ordering = PI_PS_DFS;
	    } else if (STRING_EQUAL(argv[i],"hw")) {
		option->ordering = PI_PS_FROM_FILE;
	    } else {
		goto usage;
	    }
	} else if (STRING_EQUAL(argv[i],"-order")) {
	    i++;
	    option->ordering = PI_PS_GIVEN;
	    option->orderPiPs = util_strsav(argv[i]);
	} else if (STRING_EQUAL(argv[i],"-reordering")) {
	    i++;
	    if (STRING_EQUAL(argv[i],"none")) {
		option->reordering = CUDD_REORDER_NONE;
	    } else if (STRING_EQUAL(argv[i],"random")) {
		option->reordering = CUDD_REORDER_RANDOM;
	    } else if (STRING_EQUAL(argv[i],"bernard") ||
		STRING_EQUAL(argv[i],"pivot")) {
		option->reordering = CUDD_REORDER_RANDOM_PIVOT;
	    } else if (STRING_EQUAL(argv[i],"sifting")) {
		option->reordering = CUDD_REORDER_SIFT;
	    } else if (STRING_EQUAL(argv[i],"converge")) {
		option->reordering = CUDD_REORDER_SIFT_CONVERGE;
	    } else if (STRING_EQUAL(argv[i],"symm")) {
		option->reordering = CUDD_REORDER_SYMM_SIFT;
	    } else if (STRING_EQUAL(argv[i],"cosymm")) {
		option->reordering = CUDD_REORDER_SYMM_SIFT_CONV;
	    } else if (STRING_EQUAL(argv[i],"tree") ||
		STRING_EQUAL(argv[i],"group")) {
		option->reordering = CUDD_REORDER_GROUP_SIFT;
	    } else if (STRING_EQUAL(argv[i],"cotree") ||
		STRING_EQUAL(argv[i],"cogroup")) {
		option->reordering = CUDD_REORDER_GROUP_SIFT_CONV;
	    } else if (STRING_EQUAL(argv[i],"win2")) {
		option->reordering = CUDD_REORDER_WINDOW2;
	    } else if (STRING_EQUAL(argv[i],"win3")) {
		option->reordering = CUDD_REORDER_WINDOW3;
	    } else if (STRING_EQUAL(argv[i],"win4")) {
		option->reordering = CUDD_REORDER_WINDOW4;
	    } else if (STRING_EQUAL(argv[i],"win2conv")) {
		option->reordering = CUDD_REORDER_WINDOW2_CONV;
	    } else if (STRING_EQUAL(argv[i],"win3conv")) {
		option->reordering = CUDD_REORDER_WINDOW3_CONV;
	    } else if (STRING_EQUAL(argv[i],"win4conv")) {
		option->reordering = CUDD_REORDER_WINDOW4_CONV;
	    } else if (STRING_EQUAL(argv[i],"annealing")) {
		option->reordering = CUDD_REORDER_ANNEALING;
	    } else if (STRING_EQUAL(argv[i],"genetic")) {
		option->reordering = CUDD_REORDER_GENETIC;
	    } else if (STRING_EQUAL(argv[i],"linear")) {
		option->reordering = CUDD_REORDER_LINEAR;
	    } else if (STRING_EQUAL(argv[i],"linconv")) {
		option->reordering = CUDD_REORDER_LINEAR_CONVERGE;
	    } else if (STRING_EQUAL(argv[i],"exact")) {
		option->reordering = CUDD_REORDER_EXACT;
	    } else {
		goto usage;
	    }
	} else if (STRING_EQUAL(argv[i],"-autodyn")) {
	    option->autoDyn = 3;
	} else if (STRING_EQUAL(argv[i],"-autodynB")) {
	    option->autoDyn |= 1;
	} else if (STRING_EQUAL(argv[i],"-autodynZ")) {
	    option->autoDyn |= 2;
	} else if (STRING_EQUAL(argv[i],"-automethod")) {
	    i++;
	    if (STRING_EQUAL(argv[i],"none")) {
		option->autoMethod = CUDD_REORDER_NONE;
	    } else if (STRING_EQUAL(argv[i],"random")) {
		option->autoMethod = CUDD_REORDER_RANDOM;
	    } else if (STRING_EQUAL(argv[i],"bernard") ||
		STRING_EQUAL(argv[i],"pivot")) {
		option->autoMethod = CUDD_REORDER_RANDOM_PIVOT;
	    } else if (STRING_EQUAL(argv[i],"sifting")) {
		option->autoMethod = CUDD_REORDER_SIFT;
	    } else if (STRING_EQUAL(argv[i],"converge")) {
		option->autoMethod = CUDD_REORDER_SIFT_CONVERGE;
	    } else if (STRING_EQUAL(argv[i],"symm")) {
		option->autoMethod = CUDD_REORDER_SYMM_SIFT;
	    } else if (STRING_EQUAL(argv[i],"cosymm")) {
		option->autoMethod = CUDD_REORDER_SYMM_SIFT_CONV;
	    } else if (STRING_EQUAL(argv[i],"tree") ||
		STRING_EQUAL(argv[i],"group")) {
		option->autoMethod = CUDD_REORDER_GROUP_SIFT;
	    } else if (STRING_EQUAL(argv[i],"cotree") ||
		STRING_EQUAL(argv[i],"cogroup")) {
		option->autoMethod = CUDD_REORDER_GROUP_SIFT_CONV;
	    } else if (STRING_EQUAL(argv[i],"win2")) {
		option->autoMethod = CUDD_REORDER_WINDOW2;
	    } else if (STRING_EQUAL(argv[i],"win3")) {
		option->autoMethod = CUDD_REORDER_WINDOW3;
	    } else if (STRING_EQUAL(argv[i],"win4")) {
		option->autoMethod = CUDD_REORDER_WINDOW4;
	    } else if (STRING_EQUAL(argv[i],"win2conv")) {
		option->autoMethod = CUDD_REORDER_WINDOW2_CONV;
	    } else if (STRING_EQUAL(argv[i],"win3conv")) {
		option->autoMethod = CUDD_REORDER_WINDOW3_CONV;
	    } else if (STRING_EQUAL(argv[i],"win4conv")) {
		option->autoMethod = CUDD_REORDER_WINDOW4_CONV;
	    } else if (STRING_EQUAL(argv[i],"annealing")) {
		option->autoMethod = CUDD_REORDER_ANNEALING;
	    } else if (STRING_EQUAL(argv[i],"genetic")) {
		option->autoMethod = CUDD_REORDER_GENETIC;
	    } else if (STRING_EQUAL(argv[i],"linear")) {
		option->autoMethod = CUDD_REORDER_LINEAR;
	    } else if (STRING_EQUAL(argv[i],"linconv")) {
		option->autoMethod = CUDD_REORDER_LINEAR_CONVERGE;
	    } else if (STRING_EQUAL(argv[i],"exact")) {
		option->autoMethod = CUDD_REORDER_EXACT;
	    } else {
		goto usage;
	    }
	} else if (STRING_EQUAL(argv[i],"-tree")) {
	    i++;
	    option->treefile = util_strsav(argv[i]);
	} else if (STRING_EQUAL(argv[i],"-first")) {
	    i++;
	    option->firstReorder = (int)atoi(argv[i]);
	} else if (STRING_EQUAL(argv[i],"-countdead")) {
	    option->countDead = TRUE;
	} else if (STRING_EQUAL(argv[i],"-growth")) {
	    i++;
	    option->maxGrowth = (int)atoi(argv[i]);
	} else if (STRING_EQUAL(argv[i],"-groupcheck")) {
	    i++;
	    if (STRING_EQUAL(argv[i],"check")) {
		option->groupcheck = CUDD_GROUP_CHECK;
	    } else if (STRING_EQUAL(argv[i],"nocheck")) {
		option->groupcheck = CUDD_NO_CHECK;
	    } else if (STRING_EQUAL(argv[i],"check2")) {
		option->groupcheck = CUDD_GROUP_CHECK2;
	    } else if (STRING_EQUAL(argv[i],"check3")) {
		option->groupcheck = CUDD_GROUP_CHECK3;
	    } else if (STRING_EQUAL(argv[i],"check4")) {
		option->groupcheck = CUDD_GROUP_CHECK4;
	    } else if (STRING_EQUAL(argv[i],"check5")) {
		option->groupcheck = CUDD_GROUP_CHECK5;
	    } else if (STRING_EQUAL(argv[i],"check6")) {
		option->groupcheck = CUDD_GROUP_CHECK6;
	    } else if (STRING_EQUAL(argv[i],"check7")) {
		option->groupcheck = CUDD_GROUP_CHECK7;
	    } else if (STRING_EQUAL(argv[i],"check8")) {
		option->groupcheck = CUDD_GROUP_CHECK8;
	    } else if (STRING_EQUAL(argv[i],"check9")) {
		option->groupcheck = CUDD_GROUP_CHECK9;
	    } else {
		goto usage;
	    }
	} else if (STRING_EQUAL(argv[i],"-arcviolation")) {
	    i++;
	    option->arcviolation = (int)atoi(argv[i]);
	} else if (STRING_EQUAL(argv[i],"-symmviolation")) {
	    i++;
	    option->symmviolation = (int)atoi(argv[i]);
	} else if (STRING_EQUAL(argv[i],"-recomb")) {
	    i++;
	    option->recomb = (int)atoi(argv[i]);
	} else if (STRING_EQUAL(argv[i],"-drop")) {
	    option->nodrop = FALSE;
	} else if (STRING_EQUAL(argv[i],"-sign")) {
	    option->signatures = TRUE;
	} else if (STRING_EQUAL(argv[i],"-genetic")) {
	    option->gaOnOff = 1;
	} else if (STRING_EQUAL(argv[i],"-genepop")) {
	    option->gaOnOff = 1;
	    i++;
	    option->populationSize = (int)atoi(argv[i]);
	} else if (STRING_EQUAL(argv[i],"-genexover")) {
	    option->gaOnOff = 1;
	    i++;
	    option->numberXovers = (int) atoi(argv[i]);
	} else if (STRING_EQUAL(argv[i],"-seed")) {
	    i++;
	    Cudd_Srandom((long)atoi(argv[i]));
	} else if (STRING_EQUAL(argv[i],"-dumpfile")) {
	    i++;
	    option->bdddump = TRUE;
	    option->dumpfile = util_strsav(argv[i]);
	} else if (STRING_EQUAL(argv[i],"-dumpblif")) {
	    option->dumpFmt = 1; /* blif */
	} else if (STRING_EQUAL(argv[i],"-dumpdaVinci")) {
	    option->dumpFmt = 2; /* daVinci */
	} else if (STRING_EQUAL(argv[i],"-dumpddcal")) {
	    option->dumpFmt = 3; /* DDcal */
	} else if (STRING_EQUAL(argv[i],"-dumpfact")) {
	    option->dumpFmt = 4; /* factored form */
	} else if (STRING_EQUAL(argv[i],"-store")) {
	    i++;
	    option->store = (int) atoi(argv[i]);   
	} else if (STRING_EQUAL(argv[i],"-storefile")) {
	    i++;
	    option->storefile = util_strsav(argv[i]);
	} else if (STRING_EQUAL(argv[i],"-loadfile")) {
	    i++;
	    option->load = 1;
	    option->loadfile = util_strsav(argv[i]);
	} else if (STRING_EQUAL(argv[i],"-p")) {
	    i++;
	    option->verb = (int) atoi(argv[i]);
	} else {
	    goto usage;
	}
    }

    if (option->store >= 0 && option->storefile == NULL) {
	(void) fprintf(stdout,"-storefile mandatory with -store\n");
	exit(-1);
    }

    if (option->verb >= 0) {
	(void) printf("# %s\n", NTR_VERSION);
	/* echo command line and arguments */
	(void) printf("#");
	for (i = 0; i < argc; i++) {
	    (void) printf(" %s", argv[i]);
	}
	(void) printf("\n");
        (void) printf("# CUDD Version ");
	Cudd_PrintVersion(stdout);
	(void) fflush(stdout);
    }

    return;

usage:	/* convenient goto */
    printf("Usage: please read man page\n");
    if (i == 0) {
	(void) fprintf(stdout,"too few arguments\n");
    } else {
	(void) fprintf(stdout,"option: %s is not defined\n",argv[i]);
    }
    exit(-1);

} /* end of ntrReadOptions */


/**Function********************************************************************

  Synopsis    [Reads the program options from a file.]

  Description [Reads the program options from a file. Opens file. Reads
  the command line from the otpions file using the read_line func. Scans
  the line looking for spaces, each space is a searator and demarks a
  new option.  When a space is found, it is changed to a \0 to terminate
  that string; then the next value of slot points to the next non-space
  character.  There is a limit of 1024 options.
  Should produce an error (presently doesn't) on overrun of options, but
  this is very unlikely to happen.]

  SideEffects [none]

  SeeAlso     []

******************************************************************************/
static void
ntrReadOptionsFile(
  char * name,
  char *** argv,
  int * argc)
{
    char	**slot;
    char	*line;
    char	c;
    int		index,flag;
    FILE	*fp;

    if ((fp = fopen(name,"r")) == NULL) {
	fprintf(stderr,"Error: can not find cmd file %s\n",name);
	exit(-1);
    }

    slot = ALLOC(char *,1024);
    index = 1;
    line = readLine(fp);
    flag = TRUE;
    
    do {
	c = *line;
	if ( c == ' ')  {
	    flag = TRUE;
	    *line = '\0';
	} else if ( c != ' ' && flag == TRUE) {
	    flag = FALSE;
	    slot[index] = line;
	    index++; 
	}
	line++;
    } while ( *line != '\0');

    
    *argv = slot;
    *argc = index;

    fclose(fp);

} /* end of ntrReadOptionsFile */


/**Function********************************************************************

  Synopsis    [Reads a line from the option file.]

  Description []

  SideEffects [none]

  SeeAlso     []

******************************************************************************/
static char*
readLine(
  FILE * fp)
{
    int  c;
    char *pbuffer;

    pbuffer = buffer;

    /* Strip white space from beginning of line. */
    for(;;) {
	c = getc(fp);
	if ( c == EOF) return(NULL);
	if ( c == '\n') {
	    *pbuffer = '\0';
	    return(buffer); /* got a blank line */
	}
	if ( c != ' ') break;
    }
    do {
	if ( c == '\\' ) { /* if we have a continuation character.. */
	    do {	/* scan to end of line */
		c = getc(fp);
		if ( c == '\n' ) break;
	    } while ( c != EOF);
	    if ( c != EOF) {
		*pbuffer = ' ';
		pbuffer++;
	    } else return( buffer);
	    c = getc(fp);
	    continue;
	}
	*pbuffer = c;
	pbuffer++;
	c = getc(fp);
    } while( c != '\n' &&  c != EOF);
    *pbuffer = '\0';
    return(buffer);

} /* end of readLine */


/**Function********************************************************************

  Synopsis    [Opens a file.]

  Description [Opens a file, or fails with an error message and exits.
  Allows '-' as a synonym for standard input.]

  SideEffects [None]

  SeeAlso     []

******************************************************************************/
static FILE *
open_file(
  char * filename,
  char * mode)
{
    FILE *fp;

    if (strcmp(filename, "-") == 0) {
        return mode[0] == 'r' ? stdin : stdout;
    } else if ((fp = fopen(filename, mode)) == NULL) {
        perror(filename);
        exit(1);
    }
    return(fp);

} /* end of open_file */


/**Function********************************************************************

  Synopsis    [Applies reordering to the DDs.]

  Description [Explicitly applies reordering to the DDs. Returns 1 if
  successful; 0 otherwise.]

  SideEffects [None]

  SeeAlso     []

*****************************************************************************/
static int
reorder(
  BnetNetwork * net,
  DdManager * dd /* DD Manager */,
  NtrOptions * option)
{
#ifdef DD_DEBUG
    st_table	*mintermTable;	/* minterm counts for each output */
#endif
    int result;			/* return value from functions */

    (void) printf("Number of inputs = %d\n",net->ninputs);

    /* Perform the final reordering */
    if (option->reordering != CUDD_REORDER_NONE) {
#ifdef DD_DEBUG
	result = Cudd_DebugCheck(dd);
	if (result != 0) {
	    (void) fprintf(stderr,"Error reported by Cudd_DebugCheck\n");
	    return(0);
	}
	result = Cudd_CheckKeys(dd);
	if (result != 0) {
	    (void) fprintf(stderr,"Error reported by Cudd_CheckKeys\n");
	    return(0);
	}
	mintermTable = checkMinterms(net,dd,NULL);
	if (mintermTable == NULL) exit(2);
#endif

	dd->siftMaxVar = 1000000;
	dd->siftMaxSwap = 1000000000;
	result = Cudd_ReduceHeap(dd,option->reordering,1);
	if (result == 0) return(0);
#ifdef DD_DEBUG
	result = Cudd_DebugCheck(dd);
	if (result != 0) {
	    (void) fprintf(stderr,"Error reported by Cudd_DebugCheck\n");
	    return(0);
	}
	result = Cudd_CheckKeys(dd);
	if (result != 0) {
	    (void) fprintf(stderr,"Error reported by Cudd_CheckKeys\n");
	    return(0);
	}
	mintermTable = checkMinterms(net,dd,mintermTable);
#endif

	/* Print symmetry stats if pertinent */
	if (dd->tree == NULL &&
	    (option->reordering == CUDD_REORDER_SYMM_SIFT ||
	    option->reordering == CUDD_REORDER_SYMM_SIFT_CONV))
	    Cudd_SymmProfile(dd,0,dd->size - 1);
    }

    if (option->gaOnOff) {
	result = Cudd_ReduceHeap(dd,CUDD_REORDER_GENETIC,1);
	if (result == 0) {
	    (void) printf("Something went wrong in cuddGa\n");
	    return(0);
	}
    }

    return(1);

} /* end of reorder */


/**Function********************************************************************

  Synopsis    [Frees the option structure and its appendages.]

  Description []

  SideEffects [None]

  SeeAlso     []

*****************************************************************************/
static void
freeOption(
  NtrOptions * option)
{
    if (option->file1 != NULL) FREE(option->file1);
    if (option->file2 != NULL) FREE(option->file2);
    if (option->orderPiPs != NULL) FREE(option->orderPiPs);
    if (option->treefile != NULL) FREE(option->treefile);
    if (option->sinkfile != NULL) FREE(option->sinkfile);
    if (option->dumpfile != NULL) FREE(option->dumpfile);
    if (option->loadfile != NULL) FREE(option->loadfile);
    if (option->storefile != NULL) FREE(option->storefile);
    if (option->node != NULL) FREE(option->node);
    FREE(option);

} /* end of freeOption */


/**Function********************************************************************

  Synopsis    [Starts the CUDD manager with the desired options.]

  Description [Starts the CUDD manager with the desired options.
  We start with 0 variables, because Ntr_buildDDs will create new
  variables rather than using whatever already exists.]

  SideEffects [None]

  SeeAlso     []

*****************************************************************************/
static DdManager *
startCudd(
  NtrOptions * option,
  int  nvars)
{
    DdManager *dd;
    int result;

    dd = Cudd_Init(0, 0, option->slots, option->cacheSize, option->maxMemory);
    if (dd == NULL) return(NULL);

    Cudd_SetGroupcheck(dd,option->groupcheck);
    if (option->autoDyn & 1) {
	Cudd_AutodynEnable(dd,option->autoMethod);
    }
    dd->nextDyn = option->firstReorder;
    dd->countDead = (option->countDead == FALSE) ? ~0 : 0;
    dd->maxGrowth = 1.0 + ((float) option->maxGrowth / 100.0);
    dd->recomb = option->recomb;
    dd->arcviolation = option->arcviolation;
    dd->symmviolation = option->symmviolation;
    dd->populationSize = option->populationSize;
    dd->numberXovers = option->numberXovers;
    result = ntrReadTree(dd,option->treefile,nvars);
    if (result == 0) {
	Cudd_Quit(dd);
	return(NULL);
    }
#ifndef DD_STATS
    result = Cudd_EnableReorderingReporting(dd);
    if (result == 0) {
	(void) fprintf(stderr,
		       "Error reported by Cudd_EnableReorderingReporting\n");
	Cudd_Quit(dd);
	return(NULL);
    }
#endif

    return(dd);

} /* end of startCudd */


/**Function********************************************************************

  Synopsis    [Reads the variable group tree from a file.]

  Description [Reads the variable group tree from a file.
  Returns 1 if successful; 0 otherwise.]

  SideEffects [None]

  SeeAlso     []

*****************************************************************************/
static int
ntrReadTree(
  DdManager * dd,
  char * treefile,
  int  nvars)
{
    FILE *fp;
    MtrNode *root;

    if (treefile == NULL) {
	return(1);
    }

    if ((fp = fopen(treefile,"r")) == NULL) {
	(void) fprintf(stderr,"Unable to open %s\n",treefile);
	return(0);
    }

    root = Mtr_ReadGroups(fp,ddMax(Cudd_ReadSize(dd),nvars));
    if (root == NULL) {
	return(0);
    }

    Cudd_SetTree(dd,root);

    return(1);

} /* end of ntrReadTree */
