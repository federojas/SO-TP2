#include <processManagerQueue.h>

void queueProcess(t_process_list *processes, t_process_node * process) {
  if (queueIsEmpty(processes)) {
    processes->first = process;
    processes->last = processes->first;
  } else {
    processes->last->next = (struct t_process_node *)process;
    process->next = NULL;
    processes->last = process;
  }

  if (process->pcb.state == READY) {
    processes->readySize++;
  }

  processes->size++;
}

t_process_node * dequeueProcess(t_process_list *processes) {
  if (queueIsEmpty(processes)) {
    return NULL;
  } 

  t_process_node * first = processes->first;
  processes->first = (struct process_node *)processes->first->next;
  processes->size--;

  if (first->pcb.state == READY) {
    processes->readySize--;
  }

  return first;
}


int queueIsEmpty(t_process_list *processes) {
  return processes->size == 0;
}


