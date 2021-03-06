/******************************************************************************
*	cfgDefault.h
*
*	Default cfg values for fallback.
*
*	Stuart Miller
*	Missouri S&T CS 5401
*	Fall 2017
******************************************************************************/
#ifndef CFGDEFAULT_H
#define CFGDEFAULT_H


/**********************************************************
*	Compiler Constants
**********************************************************/
#define DEFAULT_SEEDFROMTIME		(false)
#define DEFAULT_SEED				(123456789)
#define	DEFAULT_ALGORITHM			('R')
#define DEFAULT_RUNS				(1)
#define DEFAULT_FITNESSEVALS		(1)
#define DEFAULT_LOGFILE				("logs/default.txt")
#define DEFAULT_SOLUTIONFILE		("solutions/default.txt")
#define DEFAULT_INPUTFILE			("patterns/default.txt")
#define DEFAULT_SURVIVALSTRATEGY	(SURVIVALSTRAT_COMMA)
#define DEFAULT_MU					(100)
#define DEFAULT_LAMBDA				(100)
#define DEFAULT_RECOMBINATIONTYPE	(0)
#define DEFAULT_CROSSOVERS			(2)
#define DEFAULT_CROSSOVERSSA		(false)
#define DEFAULT_UNIFORMPROB			(0.50f)
#define DEFAULT_MUTATIONTYPE		(MUTATION_RANDOM)
#define DEFAULT_MUTATIONRATE		(0.20f)
#define DEFAULT_MUTATIONRATESA		(false)
#define DEFAULT_CREEPDIST			(1)
#define DEFAULT_PARENTSEL			(PARENTSEL_KTOURNAMENT)
#define DEFAULT_PARENTSELTOURNSIZE	(10)
#define DEFAULT_SURVIVORSEL			(SURVIVORSEL_KTOURNAMENT)
#define DEFAULT_SURVIVORSELTOURNSIZE (10)
#define DEFAULT_TERMTYPE			(TERMTYPE_NUMEVALS)
#define DEFAULT_CONSTRAINTSAT		(CONSTRAINTSAT_RANDOM)
#define DEFAULT_PENALTYWEIGHT		(10.0f)
#define DEFAULT_PENALTYWEIGHTSA		(PENALTYWEIGHT_FIXED)
#define DEFAULT_TERMGENSUNCHANGED	(100)
#define DEFAULT_OBJCTIVE1			(OBJECTIVE_LENGTH)
#define DEFAULT_OBJCTIVE2			(OBJECTIVE_WIDTH)


#endif
