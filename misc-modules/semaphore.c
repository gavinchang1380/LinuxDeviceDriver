#include <linux/module.h>
#include <linux/init.h>

#include <linux/sched.h>  /* current and everything */
#include <linux/kernel.h> /* printk() */
#include <linux/fs.h>     /* everything... */
#include <linux/types.h>  /* size_t */
#include <linux/semaphore.h>

MODULE_LICENSE("Dual BSD/GPL");

static int semaphore_major = 0;

static struct semaphore sema;

static ssize_t semaphore_read(struct file *filp, char __user *buf, size_t count, loff_t *pos)
{
	printk(KERN_DEBUG "process %i (%s) going to down\n",
			current->pid, current->comm);
	down_interruptible(&sema);
	printk(KERN_DEBUG "process %i (%s) is after down\n",
			current->pid, current->comm);
	return 0; /* EOF */
}

static ssize_t semaphore_write(struct file *filp, const char __user *buf, size_t count,
		loff_t *pos)
{
	printk(KERN_DEBUG "process %i (%s) going to up\n",
			current->pid, current->comm);
	up(&sema);
	return count; /* succeed, to avoid retrial */
}


static struct file_operations semaphore_fops = {
	.owner = THIS_MODULE,
	.read =  semaphore_read,
	.write = semaphore_write,
};


static int semaphore_init(void)
{
	int result;

	/*
	 * Register your major, and accept a dynamic number
	 */
	result = register_chrdev(semaphore_major, "sema", &semaphore_fops);
	if (result < 0)
		return result;
	if (semaphore_major == 0)
		semaphore_major = result; /* dynamic */

	sema_init(&sema, 1);

	return 0;
}

static void semaphore_cleanup(void)
{
	unregister_chrdev(semaphore_major, "sema");
}

module_init(semaphore_init);
module_exit(semaphore_cleanup);
