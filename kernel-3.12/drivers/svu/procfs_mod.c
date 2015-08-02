/**
 * procfs_mod.c create a "file" in /proc
 *
 */
#include <linux/module.h>       /* Specifically, a module */
#include <linux/kernel.h>       /* We're doing kernel work */
// #include <linux/proc_fs.h>
#include <../fs/proc/internal.h>
#include <linux/jiffies.h>
#include <asm/uaccess.h>        /* for copy_from_user */
#define PROCFS_MAX_SIZE         4
#define PROCFS_NAME             "assignment5"
/**
 * This structure hold information about the /proc file
 *
 */
struct proc_dir_entry *Our_Proc_File;
/**
 * The buffer used to store character for this module
 *
 */
char procfs_buffer[PROCFS_MAX_SIZE];
/**
 * The size of the buffer
 *
 */
unsigned long procfs_buffer_size = PROCFS_MAX_SIZE;
/**
 * Saved jiffies.
 *
 */
unsigned long u4oldjiffies;
/**
 * Saved system uptime.
 *
 */
unsigned long u4Uptime = 0;
ssize_t procfile_read(struct file *filp, char *buffer, size_t count, loff_t *offset);
ssize_t procfile_write(struct file *file, const char *buffer, size_t count, loff_t *offset);
struct file_operations proc_fops = {
  .owner = THIS_MODULE,
  .read = procfile_read,
  .write = procfile_write
};

/**
 * This function is called then the /proc file is read
 *
 */
ssize_t
procfile_read(struct file *filp, char *buffer, size_t count, loff_t *offset)
{
        int           ret = 0;
        unsigned long u4Interval;

        printk(KERN_INFO "procfs_mod: procfile_read (/proc/%s) called: count %d\n", PROCFS_NAME, count);
        if (count > PROCFS_MAX_SIZE) {
          printk(KERN_INFO "procfs_mod: procfile_read Finished ret %d count 0x%x Max %d\n",
                 ret, (int)count, PROCFS_MAX_SIZE);
            return -EFAULT;
        }
        if (*offset > PROCFS_MAX_SIZE) {
          /* we have finished to read, return 0 */
          printk(KERN_INFO "procfs_mod: procfile_read Finished ret %d Offset 0x%x Max %d\n",
                 ret, (int)offset, PROCFS_MAX_SIZE);
        }
        u4Interval = (jiffies - u4oldjiffies)/HZ;
        u4Uptime += u4Interval;
        u4oldjiffies = jiffies;
        printk(KERN_INFO "procfs_mod: Read Interval (jiffies 0x%x : HZ %d :Int %d Secs)\n",
               (unsigned int)jiffies, HZ, (int)u4Interval);
        printk(KERN_INFO "procfs_mod: System Uptime %d\n", (int)u4Uptime);
        /* fill the buffer, return the buffer size */
        if (copy_to_user(buffer, (char *)&u4Uptime, count) != 0) {
          printk(KERN_INFO "procfs_mod: FAILED copy_to_user: Fail %d Bytes\n", (int)procfs_buffer_size);
          return -EFAULT;
        }
        printk(KERN_INFO "procfs_mod: buffer 0x%x:%x:%x:%x:%x:%x:%x:%x\n",
               buffer[0],buffer[1],buffer[2],buffer[3],buffer[4],buffer[5],buffer[6],buffer[7]);
        ret = procfs_buffer_size;

        return ret;
}  /* procfile_read */
/**
 * This function is called with the /proc file is written
 *
 */
ssize_t procfile_write(struct file *file, const char *buffer, size_t count, loff_t *offset)
{
  /* get buffer size */
  procfs_buffer_size = count;
  if (procfs_buffer_size > PROCFS_MAX_SIZE ) {
    procfs_buffer_size = PROCFS_MAX_SIZE;
  }
  /* write data to the buffer */
  if ( copy_from_user(procfs_buffer, buffer, procfs_buffer_size) ) {
    return -EFAULT;
  }
  return procfs_buffer_size;
}  /* procfile_write */

/**
 *This function is called when the module is loaded
 *
 */
int my_init_module(void)
{
  /* create the /proc file */
//        Our_Proc_File = create_proc_entry(PROCFS_NAME, 0644, NULL);
  Our_Proc_File = proc_create(PROCFS_NAME, 0666, NULL, &proc_fops);
  if (Our_Proc_File == NULL) {
    printk(KERN_ALERT "init_module: Error: Could not initialize /proc/%s\n",
           PROCFS_NAME);
    return -ENOMEM;
  }
  printk(KERN_INFO "init_module: /proc/%s created\n", PROCFS_NAME);
  u4oldjiffies = jiffies;
  return 0;        /* everything is ok */
}
/**
 *This function is called when the module is unloaded
 *
 */
void my_cleanup_module(void)
{
  remove_proc_entry(PROCFS_NAME, NULL);
  printk(KERN_INFO "cleanup_module: /proc/%s removed\n", PROCFS_NAME);
  return;
}

module_init(my_init_module);
module_exit(my_cleanup_module);

