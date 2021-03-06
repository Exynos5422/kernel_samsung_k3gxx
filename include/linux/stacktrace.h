#ifndef __LINUX_STACKTRACE_H
#define __LINUX_STACKTRACE_H

struct task_struct;
struct pt_regs;

#ifdef CONFIG_STACKTRACE
struct task_struct;

struct stack_trace {
	unsigned int nr_entries, max_entries;
	unsigned long *entries;
	int skip;	/* input argument: How many entries to skip */
};

extern void save_stack_trace(struct stack_trace *trace);
extern void save_stack_trace_regs(struct pt_regs *regs,
				  struct stack_trace *trace);
extern void save_stack_trace_tsk(struct task_struct *tsk,
				struct stack_trace *trace);

extern void print_stack_trace(struct stack_trace *trace, int spaces);
<<<<<<< HEAD
=======
extern int  snprint_stack_trace(char *buf, int buf_len,
				struct stack_trace *trace, int spaces);
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83

#ifdef CONFIG_USER_STACKTRACE_SUPPORT
extern void save_stack_trace_user(struct stack_trace *trace);
#else
# define save_stack_trace_user(trace)              do { } while (0)
#endif

#else
# define save_stack_trace(trace)			do { } while (0)
# define save_stack_trace_tsk(tsk, trace)		do { } while (0)
# define save_stack_trace_user(trace)			do { } while (0)
# define print_stack_trace(trace, spaces)		do { } while (0)
<<<<<<< HEAD
=======
# define snprint_stack_trace(buf, len, trace, spaces)	do { } while (0)
>>>>>>> 6d6f1883acbba69770ae242bdf44b3dbabed7e83
#endif

#endif
