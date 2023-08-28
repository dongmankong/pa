/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  bool isUsed;
  char expr[100];
  int new_value;
  int old_value; 
  /* TODO: Add more members if necessary */

} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
    wp_pool[i].isUsed=false;
  }

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
WP* new_wp(){
  for(int i=0;i<NR_WP;++i){
    if(wp_pool[i].isUsed==false){
      wp_pool[i].isUsed=true;
      if(head==NULL){
        head=&wp_pool[i];
        head->next=NULL;
      }else{
        WP *tmp=head;
        while(tmp->next){
          tmp=tmp->next;
        }
        tmp->next=&wp_pool[i];
        tmp->next->next=NULL;
      }
      printf("success: NO is %d\n",i);
      return wp_pool+i;
    }
  }
  printf("have no WP,error\n");
  assert(0);
  return NULL;
}
void free_wp(WP *wp){
  WP *tmp=head;
  WP *be=NULL;
  while(tmp){
    if(tmp->NO==wp->NO){
      if(be==NULL){
        tmp->isUsed=false;
        head=tmp->next;
        tmp->next=NULL;
      }else{
        tmp->isUsed=false;
        be->next=tmp->next;
        tmp->next=NULL;
      }
      printf("free: NO is %d\n",tmp->NO);
      return ;
    }
    be=tmp;
    tmp=tmp->next;
  }
  printf("error,no wp\n");
}

void sdb_watchpoint_display(){
  WP* tmp=head;
  while(tmp){
    printf("Watchpoint.No: %d, expr = \"%s\", old_value = %d, new_value = %d\n",
                    tmp->NO, tmp->expr,tmp->old_value, tmp->new_value);
    tmp=tmp->next;
  }
}
void delete_watchpoint(int no){
  for(int i=0;i<NR_WP;++i){
    if(wp_pool[i].NO==no){
      free_wp(&wp_pool[i]);
    }
  }
}
void create_watchpoint(char* args){
  WP *wp=new_wp();
  strcpy(wp->expr,args);
  bool success=true;
  int tmp=expr(wp->expr,&success);
  if(success==false){
    printf("创建watchpoint的时候expr求值出现问题\n");
    return ;
  }
  wp->old_value=tmp;
  printf("Create watchpoint No.%d success.\n", wp-> NO);
}

void judge_watchpoint(){
  WP *wp=head;
  while(wp){
    bool success=true;
    int tmp=expr(wp->expr,&success);
    if(success){
      if(tmp!=wp->old_value){
          nemu_state.state=NEMU_STOP;
          printf("NO %d change\n",wp->NO);
      }
    }else{
      printf("expr error.\n");
      assert(0);
    }
  }
}