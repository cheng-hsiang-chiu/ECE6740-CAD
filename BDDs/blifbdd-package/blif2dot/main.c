/**CFile***********************************************************************

  FileName    [main.c]

  PackageName [blif2dot]

  Synopsis    [Main program for the blif2dot program.]

  Description []

  SeeAlso     []

  Author      [Congguang Yang]

  Copyright   [The file was created for academic use only in Department of
  Electrical and Computer Engineering, University of Massachusetts, Amherst.

  IN NO EVENT SHALL THE UNIVERSITY OF MASSACHUSETTS BE LIABLE FOR ANY DAMAGE
  CAUSED BY THE USE OF THIS SOFTWARE.]

 ******************************************************************************/

#include "bnet.h"

/*---------------------------------------------------------------------------*/
/* Constant declarations */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Stucture declarations */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Type declarations */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Variable declarations */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* Macro declarations */
/*---------------------------------------------------------------------------*/

/**AutomaticStart*************************************************************/

/*---------------------------------------------------------------------------*/
/* Static function prototypes */
/*---------------------------------------------------------------------------*/

#define ARGS(proto) proto

static int readOptions ARGS((int,char**,char**,int*));
static char* BDDlopt_GetFileHead ARGS((char*));
static int BDDlopt_Blif2Dot ARGS((BnetNetwork*,FILE*,int));

/**AutomaticEnd***************************************************************/

/*---------------------------------------------------------------------------*/
/* Definition of exported functions */
/*---------------------------------------------------------------------------*/

/**Function********************************************************************

  Synopsis    [Main program for poext]

  Description []

  SideEffects [None]

  SeeAlso     []

 ******************************************************************************/
int
main(
	 int  argc,
	 char ** argv)
{
	FILE *fp;		/* network file pointer */
	BnetNetwork *net = NULL;	/* network */
	int		exitval;	/* return value of Cudd_CheckZeroRef */
	int		ok;		/* overall return value from main() */
	int		result;		/* stores the return value of functions */
	BnetNode *node;		/* auxiliary pointer to network node */
	BnetNode *POnode;	/* New variable, to retrieve BDD manager from this PO node */
	int		i;		/* loop index */
	int		j;		/* loop index */
	int		k;
	int		level;		/* aux. var. used to print variable orders */
	char *filename;	/* Input blif file_name */
	char *dfilename;	/* Dump file names */
	FILE *dfp;
	int		separatePI = FALSE;	/* Separate PIs will create a nice drawing */


	result = readOptions(argc,argv,&filename,&separatePI);
	if (result == 0) exit(2);

	/* Read the Boolean network */
	fp = fopen(filename, "r");
	if (fp == NULL) {
		fprintf(stderr, "Circuit file read error\n");
		exit(2);
	}
	net = Bnet_ReadNetwork(fp,1);
	fclose(fp);

	if (net->nlatches != 0) {
		printf("Sorry, blif2dot only handles combinational circuits\n\n");
		exit(0);
	}

	/* Get file name */
	dfilename = ALLOC(char, 100);
	sprintf(dfilename,"%s.blif2dot.dot",BDDlopt_GetFileHead(filename));
	dfp = fopen(dfilename, "w");

	/* Print the blif in DOT format */
	result = BDDlopt_Blif2Dot(net, dfp, separatePI);
	if (result == 0) { exit(2); }

	fclose(dfp);
	FREE(dfilename);
	Bnet_FreeNetwork(net);

	exit(ok);

} /* end of main */

/**Function********************************************************************

  Synopsis    [Read Options]

  Description []

  SideEffects [None]

  SeeAlso     []

 *****************************************************************************/
static
int
readOptions(
			int  argc,
			char ** argv,
			char **filename,
			int *separatePI)
{
	int i;

	if (argc < 2) {
		printf("Usage: blif2dot -option file_name.blif\n");
		return(0);
	}

	for (i = 1; i < argc; i++) {
		if (argv[i][0] != '-' ) {
			*filename = util_strsav(argv[i]);
		}
		else if (strcmp(argv[i],"-sepPI") == 0) {
			*separatePI = TRUE;
		}
	}

	return(1);

} /* end of readOptions */

/**Function********************************************************************

  Synopsis    [Get the file head]

  Description []

  SideEffects [None]

  SeeAlso     []

 *****************************************************************************/
static
char *
BDDlopt_GetFileHead(
					char *fname)
{
	int i = 0;
	char *file_head, *q;
	FILE *tstf;

	/* Test if the file exists or not */
	if ((tstf = fopen(fname,"r")) == NULL) {
		perror(fname);
		exit(2);
	} else {
		fclose(tstf);
	}

	file_head = ALLOC(char, 20); /* The maximum length of the name of a test case */
	for (q = fname; *q != '.'; q++) {
		file_head[i] = *q;
		i++;
	}
	file_head[i] = '\0';

	return(file_head);

} /* end of BDDlopt_GetFileHead */

/**Function********************************************************************

  Synopsis    [Write the blif structure in DOT format]

  Description []

  SideEffects [None]

  SeeAlso     []

 *****************************************************************************/
static
int
BDDlopt_Blif2Dot(
				 BnetNetwork *net,
				 FILE *fp,
				 int separatePI)
{
	BnetNode *POnode, *ipnode, *node, *newnode;
	int result, i;

	fprintf(fp,"// The file is generated by blif2dot in University of Massachusetts\n");
	fprintf(fp,"//\n");

	/* Write header information */
	result = fprintf(fp,"digraph \"blif\" {\n");
	if (result == EOF) return(0);
	result = fprintf(fp,
					 "size = \"7.5,10\"\ncenter = true;\nedge [dir = none];\n");
	if (result == EOF) return(0);

	/* Write output fnodes */
	result = fprintf(fp,"{ rank = same; node [shape = box]; edge [style = invis];\n");
	if (result == EOF) return(0);
	for (i = 0; i < net->npos; i++) {
		if (!st_lookup(net->hash,net->outputs[i],(char **) &POnode)) {
			exit(2);
		}
		if (POnode->name == NULL) {
			result = fprintf(fp,"\"F%d\"", i);
		} else {
			result = fprintf(fp,"\"  %s  \"", POnode->name);
		}
		if (result == EOF) return(0);
		if (i == net->npos - 1) {
			result = fprintf(fp,"; }\n");
		} else {
			result = fprintf(fp," -> ");
		}
		if (result == EOF) return(0);
	}

	/* Write outputs -> ftree */
	for (i = 0; i < net->npos; i++) {
		if (!st_lookup(net->hash,net->outputs[i],(char **) &POnode)) {
			exit(2);
		}
		if (POnode->name == NULL) {
			result = fprintf(fp,"\"F%d\"", i);
		} else {
			result = fprintf(fp,"\"  %s  \"", POnode->name);
		}
		if (result == EOF) return(0);

		result = fprintf(fp," -> \"%lx\" [style = solid];\n",
						 ((long) POnode) / sizeof(BnetNode));
		if (result == EOF) return(0);
	}

	/* Write other nodes */
	node = net->nodes;
	while (node) {

		if (node->type == BNET_CONSTANT_NODE) {
			if (node->f == NULL) {
				fprintf(fp,"\"%lx\" [shape = plaintext, label = \"0\"];\n",
						((long) node) / sizeof(BnetNode));
			}
			else {
				fprintf(fp,"\"%lx\" [shape = plaintext, label = \"1\"];\n",
						((long) node) / sizeof(BnetNode));
			}
		}

		if (separatePI != TRUE) {
			if (node->type == BNET_INPUT_NODE) {
				fprintf(fp,"\"%lx\" [shape = plaintext, label = \"%s\"];\n",
						((long) node) / sizeof(BnetNode),node->name);
			}
		}

		if (node->type == BNET_INTERNAL_NODE || node->type == BNET_OUTPUT_NODE) {

			fprintf(fp,"\"%lx\" [label = \"%s\"];\n",((long) node)/sizeof(BnetNode),node->name);
			for (i = 0; i < node->ninp; i++) {
				if (!st_lookup(net->hash,(char *) node->inputs[i], (char **) &ipnode)) {
					printf("Network Error !\n"); return(0);
				}
				if (separatePI == TRUE) {
					if (ipnode->type != BNET_INPUT_NODE) {
						fprintf(fp,"\"%lx\" -> \"%lx\";\n",((long) node)/sizeof(BnetNode),
								((long) ipnode) / sizeof(BnetNode));
					}
					else { /* create a pointer for each PI node */
						newnode = ALLOC(BnetNode, 1);
						fprintf(fp,"\"%lx\" [shape = plaintext, label = \"%s\"];\n",
								((long) newnode) / sizeof(BnetNode),ipnode->name);

						fprintf(fp,"\"%lx\" -> \"%lx\";\n",((long) node)/sizeof(BnetNode),
								((long) newnode) / sizeof(BnetNode));

					}
				}
				else {
					fprintf(fp,"\"%lx\" -> \"%lx\";\n",((long) node)/sizeof(BnetNode),
							((long) ipnode) / sizeof(BnetNode));
				}
			}
		}

		node = node->next;
	}

	/* Write tail information. */
	result = fprintf(fp,"}\n");
	if (result == EOF) return(0);

	return(1);

} /* end of BDDlopt_Blif2Dot */

