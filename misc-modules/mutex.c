#include <linux/module.h>
#include <linux/init.h>

#include <linux/sched.h>  /* current and everything */
#include <linux/kernel.h> /* printk() */
#include <linux/fs.h>     /* everything... */
#include <linux/types.h>  /* size_t */
#include <linux/mutex.h>

MODULE_LICENSE("Dual BSD/GPL");

static int mutex_major = 0;

static DEFINE_MUTEX(mutex);

static ssize_t mutex_test_read(struct file *filp, char __user *buf, size_t count, loff_t *pos)
{
	printk(KERN_DEBUG "process %i (%s) going to mutex_lock\n",
			current->pid, current->comm);
	mutex_lock_interruptible(&mutex);
	printk(KERN_DEBUG "process %i (%s) is after mutex_lock\n",
			current->pid, current->comm);
	return 0; /* EOF */
}

static ssize_t mutex_test_write(struct file *filp, const char __user *buf, size_t count,
		loff_t *pos)
{
	printk(KERN_DEBUG "process %i (%s) going to mutex_unlock\n",
			current->pid, current->comm);
	mutex_unlock(&mutex);
	return count; /* succeed, to avoid retrial */
}


static struct file_operations mutex_test_fops = {
	.owner = THIS_MODULE,
	.read =  mutex_test_read,
	.write = mutex_test_write,
};


static int mutex_test_init(void)
{
	int result;

	/*
	 * Register your major, and accept a dynamic number
	 */
	result = register_chrdev(mutex_major, "mutex", &mutex_test_fops);
	if (result < 0)
		return result;
	if (mutex_major == 0)
		mutex_major = result; /* dynamic */
	return 0;
}

static void mutex_test_cleanup(void)
{
	unregister_chrdev(mutex_major, "mutex");
}

module_init(mutex_test_init);
module_exit(mutex_test_cleanup);
