/* INCLUDE FILE: RT_MESS.H

  INCLUDE FILE FOR THE DEFINITION OF RT MESSAGE STRUCTURES */

typedef struct
              {
                unsigned int bsw;
                unsigned int cmd;
                unsigned int wcnt    : 5;
                unsigned int subaddr : 5;
                unsigned int t_r     : 1;
                unsigned int data[32];
              } RT_MSG_DDC;

typedef struct
             {
        unsigned int subsys_flag  : 1;
        unsigned int svc_request  : 1;
        unsigned int busy         : 1;
        unsigned int db_accept    : 1;
        unsigned int stop_on_err  : 1;
        unsigned int current_area : 1;
        unsigned int mode         : 2;
               } CONFIG_BITS_DDC;


typedef struct
             {
                unsigned int bsw;
                unsigned int cmd;
                unsigned int time_tag;
                unsigned int address;
                unsigned int data[32];
                unsigned int subaddr ;
                unsigned int t_r ;
                unsigned int rt;
                unsigned int wcnt ;
            }RT_ENHCD_MSG_DDC;
