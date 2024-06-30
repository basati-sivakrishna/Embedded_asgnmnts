
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<Windows.h>


void sort_tasks(void);
void sort_tasks2(void);
void take_console_input(void);
void create_task(void);
int check_command(void);
int max_priority  = 9;
int max_priority_wait = 3;
 struct Task 
{
  int task_id;
  int task_priority;
  int pointer_context;
  char task_state[20];
  int event_id;
  struct Task * link;
};

struct Task tasks[20];
struct Task sorted_ready_tasks[20];
struct Task sorted_waiting_tasks[20];

int records = 0;
char console_input[20];
char command;
int ready_counter = 0;
int waiting_counter = 0;

int task_identity = 0;
int prior;
int pointer_context;
char state[1];
int event_identity;


int add_task(int id, int prior, int pointer, char state[], int event__id, struct Task **root, int pos)
{
   struct Task *temp = malloc(sizeof(struct Task));
    temp->task_id = id;
    temp->task_priority = prior;
    temp->pointer_context = pointer;
    temp->task_state[0] = state[0];
    temp->event_id = event__id;
    temp->link = NULL;
  if(pos == 0)
  {
        struct Task *curr = *root;
    while(curr->link != NULL)
    {
      curr = curr->link;
    }
    curr->link = temp;
  }
  if(pos > 0)
  {
  struct Task *prev = NULL;
  struct Task *curr = *root;
    while(curr->task_priority <= prior)
    {
        printf("checking\n");
        Sleep(500);
      prev = curr;
      curr = curr->link;
    }
    temp->link = curr;  
    prev->link = temp;
  }

}

void print_status(struct Task* root)
{
 char* status;
printf(" \n");
printf(" \n");
printf(" A => RUNNING   || B => READY    ||    C => WAITING\n");
printf("-----------------------------------------------------------------\n");
printf("TASK ID  || TASK PRIORITY  || TASK CONTEXT || STATE  || EVENT ID \n");
printf("-----------------------------------------------------------------\n");

 for(struct Task *l = root; l != NULL; l = l->link) {
       printf("%d     ||        %d       ||       %d     ||    %c   ||     %d         \n",\
       l->task_id, l->task_priority, l->pointer_context, l->task_state[0], l->event_id);
       Sleep(100);
    }
printf(" \n");
printf(" \n");
}

void delete_task(struct Task **ready_root, struct Task **wait_root)
{
  int id;
  char* str1;
  for(int i =0 ; i<5; i++)
  str1[i] = console_input[i+2];
  id = atoi(str1);
  struct Task *curr = *ready_root;
  struct Task *prev = NULL;
  if(id == 9999)
  {
    printf("**************CANNOT DELETE IDEAL TASK****************\n");
    return;
  }
  while(curr->task_id != id && curr->link != NULL)
  {
    prev = curr;
    curr= curr->link;
  }

  if(curr->link == NULL)
  {
    curr = *wait_root;
    while(curr->task_id != id && curr->link != NULL)
  {
    prev = curr;
    curr= curr->link;
  }
  }
prev->link = curr->link;
free(curr);
}

void ready_to_wait(struct Task **ready_root, struct Task **wait_root)
{
  int id, e_id;
  char* str1;
  char str2[3];
  //  printf(" here\n");

  for(int i =0 ; i<5; i++)
  str1[i] = console_input[i+2];
  id = atoi(str1);

  str2[0] = console_input[7];
  str2[1] = console_input[8];
  str2[2] = console_input[9];
   e_id = atoi(str2);

  struct Task *curr = *ready_root;
  struct Task *prev = NULL;
  if(curr->task_id == 9999)
  {
        printf("************************IDEAL TASK CANNOT BE SHIFTED******************\n");
        return;

  }
  while(curr->task_id != id && curr->link != NULL)
  {
    prev = curr;
    curr = curr->link;
    printf("checking\n");
    Sleep(500);
  }
  if(curr->link == NULL)
  {
    printf("************************COULD NOT FIND TASK IN READY LIST******************\n");
    return;
  }
  add_task(curr->task_id, curr->task_priority, curr->pointer_context, "C", e_id, wait_root, 1);
  prev->link = curr->link;
  free(curr);
}

void wait_to_ready(struct Task **ready_root, struct Task **wait_root)
{
int e_id;
 char str2[3];
  str2[0] = console_input[2];
  str2[1] = console_input[3];
  str2[2] = console_input[4];
  e_id = atoi(str2);
   struct Task *curr = *wait_root;
  struct Task *prev = NULL;
  while(curr->event_id != e_id && curr->link != NULL)
  {
    prev = curr;
    curr = curr->link;
    printf("checking\n");
    Sleep(500);
  }
  if(curr->link == NULL)
  {
    printf("************************COULD NOT FIND TASK IN WAIT LIST******************\n");
    return;
  }
add_task(curr->task_id, curr->task_priority, curr->pointer_context, "B", 0, ready_root, 1);
  prev->link = curr->link;
  free(curr);

}

int main(void)
{
  FILE *file;
  int j =0, k= 0;
  // attempt to open file.txt in read mode to read the file contents
  file = fopen("example1.txt", "r"); 
  
  // if the file failed to open, exit with an error message and status
  if (file == NULL)
  {
    printf("Error opening file.\n");
    return 1;
  }
  
  int read = 0;
  do 
  {
    read = fscanf(file,
                  "%d,%d,%d,%19[^,],%d",
                  &tasks[records].task_id, 
                  &tasks[records].task_priority, 
                  &tasks[records].pointer_context, 
                  tasks[records].task_state,
                  &tasks[records].event_id); 
    
    // if fscanf read 5 values from the file then we've successfully read 
    // in another record
    if (read == 5) records++;
    
    if (read != 5 && !feof(file))
    {
      printf("File format incorrect.\n");
      return 1;
    }

    if (ferror(file))
    {
      printf("Error reading file.\n");
      return 1;
    }

  } while (!feof(file));

  // close the file as we are done working with it
  fclose(file);
  
  // print out the number of records read
  printf("\n%d records read.\n\n", records);
  //seperate tasks for the ready and waiting list
  sorted_ready_tasks[0] = tasks[0];
for(int i= 1; i<records; i++)
{
  if(strcmp(tasks[i].task_state, "READY") == 0)
  {
    sorted_ready_tasks[j] = tasks[i];
    ready_counter++;
    j++;
  }
  else if(strcmp(tasks[i].task_state, "WAITING") == 0)
  {
    sorted_waiting_tasks[k] = tasks[i];
    k++;
    waiting_counter++;
  }
}

    sort_tasks();
    sort_tasks2();
  
  /*********************ready linked list making**********************/

    struct Task *ready_root = malloc(sizeof(struct Task));
    ready_root->link = NULL;
    ready_root->task_id = sorted_ready_tasks[0].task_id;
    ready_root->task_priority = sorted_ready_tasks[0].task_priority;
    ready_root->pointer_context =sorted_ready_tasks[0].pointer_context;
    ready_root->task_state[0] = 'A';
    ready_root->event_id = sorted_ready_tasks[0].event_id;

    for (int i=1; i<ready_counter; ++i) {
        add_task(sorted_ready_tasks[i].task_id, sorted_ready_tasks[i].task_priority\
        , sorted_ready_tasks[i].pointer_context, "B"\
        , sorted_ready_tasks[i].event_id, &ready_root, 0);
    }

  /*********************waiting linked list making**********************/

    struct Task* wait_root =  malloc(sizeof(struct Task));
    wait_root->link = NULL;
    wait_root->task_id = sorted_waiting_tasks[0].task_id;
    wait_root->task_priority = sorted_waiting_tasks[0].task_priority;
    wait_root->pointer_context =sorted_waiting_tasks[0].pointer_context;
    wait_root->task_state[0] = 'C';
    wait_root->event_id = sorted_waiting_tasks[0].event_id;

    for (int i=1; i < waiting_counter; ++i) {
        add_task(sorted_waiting_tasks[i].task_id, sorted_waiting_tasks[i].task_priority\
        , sorted_waiting_tasks[i].pointer_context, "C"\
        , sorted_waiting_tasks[i].event_id, &wait_root, 0);
    }
    printf("\n");
    print_status(ready_root);
    print_status(wait_root);
while(1){
LABEL: take_console_input();
switch(command) {
  case 'n' : printf("create task command \n");
             create_task();
             struct Task* check = ready_root;
             while (check->link != NULL)
             {
              if(check->task_id==task_identity)
              {
                 printf("***********************EXISTING TASK ID******************\n");
                 goto LABEL;

              }
              check = check->link;
             }
             check = wait_root;
             while (check->link != NULL)
             {
              if(check->task_id==task_identity)
              {
                printf("***********************EXISTING TASK ID******************\n");
                goto LABEL;
                // take_console_input();
              }
              check = check->link;
             }
             if(state[0] == 'B')
             {
              if(max_priority <= prior)
              {
             add_task(task_identity, prior, pointer_context, state, event_identity, &ready_root, 0);
             max_priority = prior;
              }
             else
             add_task(task_identity, prior, pointer_context, state, event_identity, &ready_root, 1);
             }
             else
             {
             if(max_priority_wait <= prior){
             add_task(task_identity, prior, pointer_context, state, event_identity, &wait_root, 0);
             max_priority_wait = prior;
             }
             else
             add_task(task_identity, prior, pointer_context, state, event_identity, &wait_root, 1);
             }
             break;
  case 'd' : printf("delete task \n");
            delete_task( &ready_root, &wait_root);
             break;
  case 'w' : printf("move task to waiting list command \n");
             ready_to_wait(&ready_root, &wait_root);
             break; 
  case 'e' : printf("trigger task command \n");
             wait_to_ready(&ready_root, &wait_root);
             break;
  case 's' : printf("suspend the task\n");
                struct Task* curr = ready_root;
                int e_id;
                char str1[3];
                str1[0] = console_input[2];
                str1[1] = console_input[3];
                str1[2] = console_input[4];
                e_id = atoi(str1);
                event_identity = e_id;
                task_identity = curr->task_id;
                prior = curr->task_priority;
                pointer_context = curr->pointer_context;
                state[0] = 'C';
                if(ready_root->link == NULL)
                {
                printf("YOU CANNOT DELETE IDEAL TASK\n");
                break;
                }
                ready_root = ready_root->link;
                ready_root->task_state[0] ='A' ;
                if(max_priority_wait <= prior){
                add_task(task_identity, prior, pointer_context, state, event_identity, &wait_root, 0);
                max_priority_wait = prior;
                }
                else
                add_task(task_identity, prior, pointer_context, state, event_identity, &wait_root, 1);
                free(curr);
                break;
  default  : printf("invalid task command \n"); 
            break;
}
    print_status(ready_root);
    print_status(wait_root);
}
  return 0;
}

void sort_tasks(void)
{
  int j = 0, k = 0;
  struct Task temp;
for(int i = 0; i<(ready_counter - 1); i++)
{
  for(int x =0; x< (ready_counter - 1); x++)
  {
    if(sorted_ready_tasks[x].task_priority > sorted_ready_tasks[x+1].task_priority)
    {
    temp = sorted_ready_tasks[x];
    sorted_ready_tasks[x] = sorted_ready_tasks[x+1];
    sorted_ready_tasks[x+1] = temp;
  }
  }
}
/*
printf("sorted ready tasks: \n");
for(int i = 0; i<ready_counter; i++)
{
printf("%d %d %d %s %d\n", 
           sorted_ready_tasks[i].task_id, 
           sorted_ready_tasks[i].task_priority,
           sorted_ready_tasks[i].pointer_context,
           sorted_ready_tasks[i].task_state,
           sorted_ready_tasks[i].event_id);
  printf("\n");
}
printf("ready counter value = %d\n", ready_counter);
*/
}

void sort_tasks2(void)
{
  int j = 0, k = 0;
  struct Task temp;
for(int i = 0; i<(waiting_counter - 1); i++)
{
  for(int x =0; x< (waiting_counter - 1); x++)
  {
    if(sorted_waiting_tasks[x].task_priority > sorted_waiting_tasks[x+1].task_priority)
    {
    temp = sorted_waiting_tasks[x];
    sorted_waiting_tasks[x] = sorted_waiting_tasks[x+1];
    sorted_waiting_tasks[x+1] = temp;
  }
  }
}
/*
printf("sorted ready tasks: \n");
for(int i = 0; i<ready_counter; i++)
{
printf("%d %d %d %s %d\n", 
           sorted_ready_tasks[i].task_id, 
           sorted_ready_tasks[i].task_priority,
           sorted_ready_tasks[i].pointer_context,
           sorted_ready_tasks[i].task_state,
           sorted_ready_tasks[i].event_id);
  printf("\n");
}
printf("ready counter value = %d\n", ready_counter);
*/
}

void take_console_input(void)
{
  printf("enter the command\n");
  // scanf("%[^\n]s", console_input);
  gets(console_input);
  printf("entered command is %s\n", console_input);
  if(check_command())
  {
    printf("invalid command\n");
    take_console_input();
  }
  // command = console_input[0];
}

int check_command(void)
{
command = console_input[0];
if(command == 'n' || command == 'w' || command == 'e' || command == 's' || command == 'd')
return 0;
return 1;
}

void create_task(void)
{
  char* str1;
  for(int i =0 ; i<5; i++)
  str1[i] = console_input[i+2];
  task_identity = atoi(str1);
    // Taking input for prior
    prior = 11;
    while(prior >= 10)
    {
    printf("Enter priority(should be less than 10): ");
    scanf("%d", &prior);
    }
    // Taking input for pointer_context
    printf("Enter pointer_context: ");
    scanf("%d", &pointer_context);

    // Taking input for state
    printf("Enter state: ");
    scanf(" %c", &state);  // Note the space before %c to consume any previous newline character

    // Taking input for event_id
    event_identity = 0;
    if(state[0]!='B')
    {
    printf("Enter event_id: ");
    scanf("%d", &event_identity);
    }
    
/*
    // Displaying the entered values
    printf("\nYou entered:\n");
    printf("task_identity: %d\n", task_identity);
    printf("prior: %d\n", prior);
    printf("pointer_context: %d\n", pointer_context);
    printf("state: %c\n", state);
    printf("event_id: %d\n", event_identity);

*/
}
