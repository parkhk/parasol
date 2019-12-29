#include <linux/fs.h>
#include <linux/hugetlb.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/mmzone.h>
#include <linux/proc_fs.h>
#include <linux/quicklist.h>
#include <linux/seq_file.h>
#include <linux/swap.h>
#include <linux/vmstat.h>
#include <linux/atomic.h>
#include <linux/vmalloc.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#include "internal.h"

static int parasol_proc_show(struct seq_file *m, void *v)
{
	struct zone *zone, *n_zone;
	unsigned long total = 0;
	unsigned int i;
	unsigned int seg=0;

	seq_printf(m, "Buddy Segment Infomation\n");
	/*
	seq_printf(m, "%16s ", "Zone Name ");
	for_each_zone(zone){
		seq_printf(m, ": %8s ", zone->name);
		if(zone->zone_start_pfn == 4096)
			n_zone = zone;
	}
	seq_printf(m, "\n%16s ", "total_bank_cnt");
	for_each_zone(zone){
		seq_printf(m, ": %8lu ", zone->total_page_cnt);
	}*/
	for_each_zone(zone){
		if(zone->zone_start_pfn == 4096)
			n_zone = zone;
	}
	for (i = 0; i < NUM_OF_BANK; i++){
		if(!(i%64)){
			seq_printf(m, "\n");
		}
		seq_printf(m, ": %8lu ",	n_zone->page_cnt_bank[i]);
		total+=n_zone->page_cnt_bank[i];
	}
	/*for (i = 0; i < NUM_OF_BANK; i++){
		if(!(i%16)){
			seq_printf(m, "\n\n%u segment", seg++);
		}
		seq_printf(m, "\n%3u %12s ", i%16, "bank");
		for_each_zone(zone){
			seq_printf(m, ": %8lu ",	zone->page_cnt_bank[i]);
			total+=zone->page_cnt_bank[i];
		}
	}*/
	seq_printf(m, "\nSum = %lu(zone->total_page_cnt : %lu)\n", total, n_zone->total_page_cnt);

	return 0;
}

static int parasol_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, parasol_proc_show, NULL);
}

static const struct file_operations parasol_proc_fops = {
	.open		= parasol_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int __init proc_parasol_init(void)
{
	proc_create("parasol", 0, NULL, &parasol_proc_fops);
	return 0;
}
module_init(proc_parasol_init);
