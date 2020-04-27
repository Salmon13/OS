#include <linux/kernel.h>
#include <linux/linkage.h>

asmlinkage int sys_my_print(unsigned long st_sec, unsigned long st_nsec, unsigned long ed_sec, unsigned long ed_nsec, int pid) {

	printk("[Project1] %d %lu.%09lu %lu.%09lu\n", pid, st_sec, st_nsec, ed_sec, ed_nsec);

	return 0;	
}