/*
 *  procfs1.c -  create a "file" in /proc
 *
 */

#include <linux/module.h>	/* Specifically, a module */
#include <linux/kernel.h>	/* We're doing kernel work */
#include <linux/proc_fs.h>	/* Necessary because we use the proc fs */

#define procfs_name "helloworld"

/**
 * This structure hold information about the /proc file
 *
 */
struct proc_dir_entry *Our_Proc_File;

/* Put data into the proc fs file.
 * 
 * Arguments
 * =========
 * 1. The buffer where the data is to be inserted, if
 *    you decide to use it.
 * 2. A pointer to a pointer to characters. This is	//指针的指针, by Tom Xue
 *    useful if you don't want to use the buffer
 *    allocated by the kernel.
 * 3. The current position in the file
 * 4. The size of the buffer in the first argument.
 * 5. Write a "1" here to indicate EOF.
 * 6. A pointer to data (useful in case one common 
 *    read for multiple /proc/... entries)
 *
 * Usage and Return Value
 * ======================
 * A return value of zero means you have no further
 * information at this time (end of file). A negative
 * return value is an error condition.
 *
 * For More Information
 * ====================
 * The way I discovered what to do with this function
 * wasn't by reading documentation, but by reading the
 * code which used it. I just looked to see what uses
 * the get_info field of proc_dir_entry struct (I used a
 * combination of find and grep, if you're interested),
 * and I saw that it is used in <kernel source
 * directory>/fs/proc/array.c.
 *
 * If something is unknown about the kernel, this is
 * usually the way to go. In Linux we have the great
 * advantage of having the kernel source code for
 * free - use it.
 */
int
procfile_read(char *buffer,
	      char **buffer_location,
	      off_t offset, int buffer_length, int *eof, void *data) //refer to proc_dir_entry->read_proc(read_proc_t), by Tom Xue
{
	int ret;
	
	printk(KERN_INFO "procfile_read (/proc/%s) called\n", procfs_name);
	
	/* 
	 * We give all of our information in one go, so if the
	 * user asks us if we have more information the
	 * answer should always be no.
	 *
	 * This is important because the standard read
	 * function from the library would continue to issue
	 * the read system call until the kernel replies
	 * that it has no more information, or until its
	 * buffer is filled.
	 */
	if (offset > 0) {
        printk(KERN_ALERT "offset = %d\n", offset);     //step 2: offset = 12, after "HellowWorld!" was written into the buffer
                                                        //step 4: while "HelloWorld!" was sending to screen(a longer process), this branch was executed again
		/* we have finished to read, return 0 解释在上面: The current position in the file, by Tom Xue */
		ret  = 0;
	} else {
        printk(KERN_ALERT "offset = %d\n", offset);     //step 1: offset = 0, at first there is nothing in the buffer
		/* fill the buffer, return the buffer size */
		//proc_register -> proc_file_operations -> read -> proc_file_read -> __proc_file_read -> read_proc, n -= copy_to_user(buf, start < page ? page : start, n);
		ret = sprintf(buffer, "HelloWorld!\n");         //step 3: "HelloWorld!" was printed out to screen, read_proc will read
	}

	return ret;
}

int init_module()
{
	Our_Proc_File = create_proc_entry(procfs_name, 0644, NULL);
	
	if (Our_Proc_File == NULL) {
		remove_proc_entry(procfs_name, NULL);
		printk(KERN_ALERT "Error: Could not initialize /proc/%s\n",
		       procfs_name);
		return -ENOMEM;
	}

	Our_Proc_File->read_proc = procfile_read;
//	Our_Proc_File->owner 	 = THIS_MODULE;
//	#define S_IRUGO		(S_IRUSR|S_IRGRP|S_IROTH)
	Our_Proc_File->mode 	 = S_IFREG | S_IRUGO; //regular file | user/group/others readable, by Tom Xue
	Our_Proc_File->uid 	 = 0;
	Our_Proc_File->gid 	 = 0;
	Our_Proc_File->size 	 = 37;

	printk(KERN_INFO "/proc/%s created\n", procfs_name);	
	return 0;	/* everything is ok */
}

void cleanup_module()
{
	remove_proc_entry(procfs_name, NULL);
	printk(KERN_INFO "/proc/%s removed\n", procfs_name);
}
/* run on N9 and output is below:
RM696-51-1_PR_003:~# insmod procfs1.ko 
RM696-51-1_PR_003:~# dmesg | tail
...
[42073.866485] procfs1: module license 'unspecified' taints kernel.
[42073.866973] Disabling lock debugging due to kernel taint
[42073.876953] /proc/helloworld created

RM696-51-1_PR_003:~# ls /proc/helloworld 
/proc/helloworld

RM696-51-1_PR_003:~# cat /proc/helloworld 
HelloWorld!

RM696-51-1_PR_003:~# dmesg | tail
...
[42073.866485] procfs1: module license 'unspecified' taints kernel.
[42073.866973] Disabling lock debugging due to kernel taint
[42073.876953] /proc/helloworld created
[42119.118652] procfile_read (/proc/helloworld) called
[42119.118835] procfile_read (/proc/helloworld) called
[42119.123107] procfile_read (/proc/helloworld) called
*/
