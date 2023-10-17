#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/jiffies.h>

/* Meta Information */


#define BUFFER_SIZE 128
#define PROC_NAME "seconds"
long int j;
static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos);

static struct file_operations proc_ops = {
    .owner = THIS_MODULE,
    .read = proc_read,
};
/* This function is called when the module is loaded. */
int proc_init(void) {
    j = jiffies;
        /* creates the /proc/jiffies entry */
        proc_create(PROC_NAME, 0666, NULL, &proc_ops);
        return 0;
}

/* This function is called when the module is removed. */
void proc_exit (void) {

        /* removes the /proc/jiffies entry */
        remove_proc_entry(PROC_NAME, NULL);
}

/* This function is called each time /proc/jiffies is read */
ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
int rv = 0;
char buffer[BUFFER_SIZE];
static int completed = 0;

if (completed) {
completed = 0;
return 0;
}

completed = 1;
/* print jiffies */
rv = sprintf(buffer, "SECONDS: %lu\n",(jiffies - j)/HZ);
/* copies kernel space buffer to user space usr buf */
copy_to_user(usr_buf, buffer, rv);
return rv;
}

/* Macros for registering module entry and exit points. */
module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Soutrik");
MODULE_DESCRIPTION("Assignment_1 part_2");