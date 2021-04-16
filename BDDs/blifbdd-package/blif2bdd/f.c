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

