#include "lwip_comm.h"


//__lwip_dev lwipdev;
struct netif enc28j60_netif;

void Lwip_task(void *arg)
{
	Lwip_init();

}

void Lwip_init(void)
{
	//����ip�����ء����������ַ�ṹ��
	struct ip_addr ip_addr,gw_addr,netmask;
	
	//��ʼ��lwip�ں�
	lwip_init();
	DEBUG_INFO("lwip_init success\n");
	
	//���ip�����ء����������ַ
	IP4_ADDR(&ip_addr, 192,168,1,38);
	IP4_ADDR(&gw_addr, 192,168,1,1);
	IP4_ADDR(&netmask, 255,255,255,0);
	
	//������ڵ�Ĭ������
	netif_add(&enc28j60_netif,&ip_addr,&netmask,&gw_addr,NULL,ethernetif_init,ethernet_input);
	netif_set_default(&enc28j60_netif);
	netif_set_up(&enc28j60_netif);

}

