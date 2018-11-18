#include "lwip_comm.h"


//__lwip_dev lwipdev;
struct netif enc28j60_netif;

void Lwip_task(void *arg)
{
	Lwip_init();

}

void Lwip_init(void)
{
	//定义ip、网关、子网掩码地址结构体
	struct ip_addr ip_addr,gw_addr,netmask;
	
	//初始化lwip内核
	lwip_init();
	DEBUG_INFO("lwip_init success\n");
	
	//添加ip、网关、子网掩码地址
	IP4_ADDR(&ip_addr, 192,168,1,38);
	IP4_ADDR(&gw_addr, 192,168,1,1);
	IP4_ADDR(&netmask, 255,255,255,0);
	
	//添加网口到默认设置
	netif_add(&enc28j60_netif,&ip_addr,&netmask,&gw_addr,NULL,ethernetif_init,ethernet_input);
	netif_set_default(&enc28j60_netif);
	netif_set_up(&enc28j60_netif);

}

