#ifndef QL_WEB_H
#define QL_WEB_H



int ql_web_on(void);

int ql_web_off(void);

void ql_dns_fake_server_init(void * ipaddr); 

void ql_dns_fake_server_deinit(void);

#endif
