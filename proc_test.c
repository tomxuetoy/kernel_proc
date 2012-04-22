/*
 * =====================================================================================
 *
 *       Filename:  proc_test.c
 *
 *    Description:  to show a simple /proc method
 *
 *        Version:  1.0
 *        Created:  04/22/12 00:11:31
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Tom Xue (), tom.xue@nokia.com
 *        Company:  Nokia   
 *
 * =====================================================================================
 */

#include <linux/module.h>   
#include <linux/init.h>
#include <linux/proc_fs.h>  

static int __init myproc_init(void)
{
        struct proc_dir_entry *entry;
        printk(KERN_ALERT "Tom Xue start to create_proc_read_entry\n");

        entry = create_proc_entry("my_proc", 0644, NULL);
        if (entry==NULL)
        {
                printk(KERN_ALERT "create_proc_read_entry error, Tom Xue!\n");
        }
        else
                printk(KERN_ALERT "create_proc_read_entry OK, Tom Xue!\n");
        
        return 0;
}
static void __exit myproc_exit(void)
{
        remove_proc_entry("my_proc", NULL);
        printk(KERN_ALERT "Good bye, my_proc, Tom Xue!\n");
}

module_init(myproc_init);
module_exit(myproc_exit);   
MODULE_LICENSE("GPL");
