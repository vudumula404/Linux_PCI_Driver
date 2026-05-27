/* INCLUDE FILE: INT.H

  INCLUDE FILE FOR THE INTERRUPT MASK DATA STRUCTURE */

typedef struct {
                unsigned int bc_eom:1;
                unsigned int fmterr_statset:1;
                unsigned int eom:1;
                } INT_DEF;



/* this structure defines the interrupt status register fields */
typedef struct {
                 int master;
                 int rt_parity;
                 int ttag_rllovr;
                 int rt_cirbuff_rllovr;
                 int rt_sacntrlwrd_eom;
                 int bc_eof;
                 int fmt_err;
                 int status_set;
                 int eom;
                } INTSTAT_REG;

