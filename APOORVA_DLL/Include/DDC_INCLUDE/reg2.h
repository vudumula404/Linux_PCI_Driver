/* THIS FILE CONTAINS STRUCTURES FOR THE SECOND CONFIGURATION REGISTER
   ON THE BUS-61580 DEMO CARD */

typedef struct
         {
             int b_disable;
             int ttag_res;
             int clr_ttag_on_sync;
             int load_ttag_on_sync;
             int intstat_autoclear;
             int lev_pulse_select;
             int clr_servreq;
             int enhanced_rtmm;
             int sep_bcst_data;
            } CFG2;

