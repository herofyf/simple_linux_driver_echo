/*le    hello.c
 * @author  Derek Molloy
 * @date    4 April 2015
 * @version 0.1
 * @brief  An introductory "Hello World!" loadable kernel module (LKM) that can display a message
 * in the /var/log/kern.log file when the module is loaded and removed. The module can accept an
 * argument when it is loaded -- the name, which appears in the kernel log files.
 * @see http://www.derekmolloy.ie/ for a full description and follow-up descriptions.
*/

#include <linux/init.h>             // Macros used to mark up functions e.g., __init __exit
#include <linux/module.h>           // Core header for loading LKMs into the kernel
#include <linux/kernel.h>           // Contains types, macros, functions for the kernel
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>
#include <linux/device.h>

#define DEVICE_NAME "ebbchar"
#define CLASS_NAME "ebb"

MODULE_LICENSE("GPL");              ///< The license type -- this affects runtime behavior
MODULE_AUTHOR("Derek Molloy");      ///< The author -- visible when you use modinfo
MODULE_DESCRIPTION("A simple Linux driver for the BBB.");  ///< The description -- see modinfo
MODULE_VERSION("0.1");              ///< The version of the module

static char *name = "world";        ///< An example LKM argument -- default value is "world"
module_param(name, charp, S_IRUGO); ///< Param desc. charp = char ptr, S_IRUGO can be read/not changed
MODULE_PARM_DESC(name, "The name to display in /var/log/kern.log");  ///< parameter description

static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops = 
{
	.open = dev_open,
	.read = dev_read,
	.write = dev_write,
	.release = dev_release,
};

char *msg = NULL;
#define MSG_SIZE 256

static int dev_open(struct inode *inodep, struct file *filep)
{
	printk(KERN_INFO "EBBChar: Device has been opend %d time(s)\n", 2);

	msg = kmalloc(sizeof(char) * MSG_SIZE, GFP_KERNEL);
	return 0;
}

static int dev_release(struct inode *inodep, struct file *filep)
{
	kfree(msg);


	return 0;
}


static ssize_t dev_read(struct file *filep, char * buf, size_t size, loff_t * offset)
{
	
	return 0;	
}

static ssize_t dev_write(struct file *filep, const char * buffer, size_t len, loff_t * offset)
{
	memset(msg, 0x0, MSG_SIZE); 

	sprintf(msg, "%s(%d letters)", buffer, len);

	int nlen = strlen(msg);
	printk(KERN_INFO "EBBChar: Received %u characters from the user\n", nlen);
	return nlen;
}

static int majorNumber;
static struct class* ebbcharClass = NULL;
static struct device* ebbcharDevice = NULL;

/** @brief The LKM initialization function
 *  The static keyword restricts the visibility of the function to within this C file. The __init
 *  macro means that for a built-in driver (not a LKM) the function is only used at initialization
 *  time and that it can be discarded and its memory freed up after that point.
 *  @return returns 0 if successful
 */
static int __init helloBBB_init(void){
   printk(KERN_INFO "EBB: Hello %s from the BBB LKM!\n", name);

   majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
   if (majorNumber < 0) {
	   printk(KERN_ALERT "EBBChar failed to register a major number\n");
	   return majorNumber;
   }

   printk(KERN_INFO "EBBChar: register correctly with major number :%d\n", majorNumber);

   // register the device class
   ebbcharClass = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(ebbcharClass)) {
	   unregister_chrdev(majorNumber, DEVICE_NAME);
	   printk(KERN_ALERT  "Failed to register device class\n");
	   return PTR_ERR(ebbcharClass);
   }


   printk(KERN_INFO "EBBChar: device class registered correctly\n");

   ebbcharDevice = device_create(ebbcharClass, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
   if (IS_ERR(ebbcharDevice)) {
	   class_destroy(ebbcharClass);
	   unregister_chrdev(majorNumber, DEVICE_NAME);
	   printk(KERN_ALERT "Failed to create the device\n");
	   return PTR_ERR(ebbcharDevice);
   }

   printk(KERN_INFO "EBBChar: device class created correctly\n");
   return 0;
}

/** @brief The LKM cleanup function
 *  Similar to the initialization function, it is static. The __exit macro notifies that if this
 *  code is used for a built-in driver (not a LKM) that this function is not required.
 */
static void __exit helloBBB_exit(void){
	device_destroy(ebbcharClass, MKDEV(majorNumber, 0));
	class_unregister(ebbcharClass);
	class_destroy(ebbcharClass);
	unregister_chrdev(majorNumber, DEVICE_NAME);

	printk(KERN_INFO "EBB: Goodbye %s from the BBB LKM!\n", name);
}

/** @brief A module must use the module_init() module_exit() macros from linux/init.h, which
 *  identify the initialization function at insertion time and the cleanup function (as
 *  listed above)
 */
module_init(helloBBB_init);
module_exit(helloBBB_exit);

