#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <signal.h>
#include "bootp.h"
#include </usr/include/mysql/mysql.h>


#define BUFF_SIZE 4096
#define TRUE 1
#define DB_HOST "127.0.0.1"
#define DB_USER "root"
#define DB_PASSWD "1234"
#define DB_NAME "IP_Management"


MYSQL *connection = NULL; MYSQL conn;
MYSQL_RES *result;  MYSQL_ROW row;  MYSQL_RES *result2; MYSQL_ROW row2;
int query;  char start_ip_addr[20]; char end_ip_addr[20];   char network[20]; char subnet[20]; char gateway[20];
char name_server[20]; char default_lease_time[10]; char max_lease_time[10]; int num;    char buf[255];             
int CountIp();

int main(int argc, char *argv[])
{

    /*
    int pid;
    pid = fork();
    if(pid < 0) {
        printf("fork error: return is [%d]\n", pid);
        perror("fork error: ");   
        exit(0);
    }else if(pid > 0) {
        printf("child process: [%d] - parent process: [%d]\n", pid,getpid());
        exit(0);
    }else if(pid == 0) {
        printf("process: [%d]\n", getpid());
    }
    signal(SIGHUP, SIG_IGN);
    chdir("/");
    setsid();
    */
   
    unsigned char packet[4096];
    //raw socket setting
    int raw_socket; int on = 1;  
    struct iphdr *iphdr;    struct udphdr *udphdr;  struct bootp *bootp;
    struct sockaddr_in address;
    //raw_socket = socket(AF_INET, SOCK_RAW,IPPROTO_UDP);
    //setsockopt(raw_socket,IPPROTO_IP, IP_HDRINCL, (char *)&on, sizeof(on));
    
   // iphdr = (struct iphdr *)packet; 
    //udphdr = (struct udphdr *)(packet + sizeof(struct iphdr));
   // bootp = (struct bootp *)(packet + sizeof(struct iphdr) + sizeof(struct udphdr));
    
    //socket setting
    int server_socket,option;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_size;  int str_len;
    char buff_rcv[BUFF_SIZE];

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if(server_socket == -1) {
    printf("socket 생성 실패");
    exit(1);
     }
    option = TRUE;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(67); //server port number
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); //32bit IPV4
    
    /*memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(68); //server port number
    address.sin_addr.s_addr = htonl(INADDR_ANY); //32bit IPV4
*/


    if(bind(server_socket, (struct sockaddr* )&server_addr, sizeof(server_addr))== -1) {
      printf("bind() 실행 에러\n");
      exit(1);
    }    
     while(1) {                                                                                                                                                                                
        str_len = recvfrom(server_socket, buff_rcv, BUFF_SIZE, 0, (struct sockaddr *)&client_addr, &client_size);
        if(strcmp(inet_ntoa(client_addr.sin_addr), "0.0.0.0") == 0 && ntohs(client_addr.sin_port) == 0);
        else {
            memset(packet, 0, 4096); //packet init
           /*
             iphdr->version = 4;
            iphdr->ihl = 5;
            iphdr->tos = 0x10;
            iphdr->protocol = IPPROTO_UDP;
            iphdr->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(struct bootp);
            iphdr->id = 0;
            iphdr->ttl = 60;
            //iphdr->saddr = inet_addr(inet_ntoa(client_addr.sin_addr));
            iphdr->saddr = inet_addr("196.120.56.3");
            iphdr->daddr = inet_addr("196.120.56.100");
            udphdr->source = htons(67);
            udphdr->dest = htons(68);
            udphdr->len = htons(8 + sizeof(struct bootp));
            udphdr->check = 1;

            bootp->bp_op = 0x02;    
            bootp->bp_htype = 0x01;
            bootp->bp_hlen = 0x06;
            bootp->bp_hops = 0;
            bootp->bp_xid = htonl(20000);
            bootp->bp_secs = 0;
            bootp->bp_flags = 0x80;
           // bootp->bp_ciaddr = inet_addr("196.120.56.3");
            bootp->bp_yiaddr = inet_addr("196.120.56.100");
            bootp->bp_vend[0] = 0x63;
            bootp->bp_vend[1] = 0x82;
            bootp->bp_vend[2] = 0x53;
            bootp->bp_vend[3] = 0x63;

            //DHCP MESSAGE TYPE
            bootp->dhcp_message.opt_type = 0x35;
            bootp->dhcp_message.opt_len = 1;
            bootp->dhcp_message.opt_value= 2;

            //DHCP SERVER IDENTIFIER 
            bootp->dhcp_server.opt_type = 54;
            bootp->dhcp_server.opt_len = 4;
            bootp->dhcp_server.opt_value = inet_addr("196.120.56.3");

            // DHCP LEASE TIME
            bootp->dhcp_lease.opt_type = 51;
            bootp->dhcp_lease.opt_len = 4;
     // phdr->saddr = inet_addr("196.120.56.3");
          bootp->dhcp_lease.opt_value = htonl(600);

            bootp->dhcp_end.opt_type = 0xff;
*/
            sendto(raw_socket, packet, iphdr->tot_len, 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
            
        }   }


 /*   
    memset(packet, 0, 4096); //packet init
    iphdr->version = 4;
    iphdr->ihl = 5;
    iphdr->tos = 0;
    iphdr->protocol = IPPROTO_UDP;
    iphdr->tot_len = sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(struct bootp);
    iphdr->id = htonl(10000);
    iphdr->ttl = 60;
    iphdr->check = 1;
    iphdr->saddr = inet_addr("123.132.123.123");
    iphdr->daddr = inet_addr("127.0.0.1");
 
    udphdr->source = htons(67);
    udphdr->dest = htons(68);
    udphdr->len = htons(8 + sizeof(struct bootp));
    udphdr->check = 1;
   

    address.sin_family = AF_INET;
    address.sin_port = htons(67);
    address.sin_addr.s_addr = inet_addr("127.0.0.1");

    bootp->bp_op = 0x02;    
    bootp->bp_htype = 0x01;
    bootp->bp_hlen = 0x06;
    bootp->bp_hops = 0;
    bootp->bp_xid = htonl(20000);
    bootp->bp_secs = 0;
    bootp->bp_flags = 0x80;
    bootp->bp_ciaddr = inet_addr("127.1.2.0");
    bootp->bp_vend[0] = 0x63;
    bootp->bp_vend[1] = 0x82;
    bootp->bp_vend[2] = 0x53;
    bootp->bp_vend[3] = 0x63;
    
    //DHCP MESSAGE TYPE
    bootp->dhcp_message.opt_type = 0x35;
    bootp->dhcp_message.opt_len = 1;
    bootp->dhcp_message.opt_value= 2;
   
     //DHCP SERVER IDENTIFIER 
    bootp->dhcp_server.opt_type = 54;
    bootp->dhcp_server.opt_len = 4;
    bootp->dhcp_server.opt_value = inet_addr("127.0.0.2");
    
    // DHCP LEASE TIME
    bootp->dhcp_lease.opt_type = 51;
    bootp->dhcp_lease.opt_len = 4;
    bootp->dhcp_lease.opt_value = htonl(600);
        
    bootp->dhcp_end.opt_type = 0xff;

    sendto(raw_socket, packet, iphdr->tot_len, 0, (struct sockaddr *)&address, sizeof(address));
    */
    /*  int server_socket, send_socket ,option, value;
      struct sockaddr_in server_addr, client_addr;
      socklen_t client_size;
      int str_len;

      char buff_rcv[BUFF_SIZE];

      server_socket = socket(AF_INET, SOCK_DGRAM, 0);
      if(server_socket == -1) {
      printf("socket 생성 실패");
      exit(1);
      }

      send_socket = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
      if(send_socket == -1) {
            printf("send socket 생성 실패");
            exit(1);
      }

      value, option = TRUE;
      setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
        ////////수정
      memset(&server_addr, 0, sizeof(server_addr));
      server_addr.sin_family = AF_INET;
      server_addr.sin_port = htons(67); //server port number
      server_addr.sin_addr.s_addr = htonl(INADDR_ANY); //32bit IPV4


      if(bind(server_socket, (struct sockaddr* )&server_addr, sizeof(server_addr))== -1) {
      printf("bind() 실행 에러\n");
      exit(1);
      }

    while(1) {
      str_len = recvfrom(server_socket, buff_rcv, BUFF_SIZE, 0, (struct sockaddr *)&client_addr, &client_size);
      if(strcmp(inet_ntoa(client_addr.sin_addr), "0.0.0.0") == 0 && ntohs(client_addr.sin_port) == 0);
      else {    
                  
      }  
 }



    //    int query;  char start_ip_addr[20]; char end_ip_addr[20];   char network[20]; char subnet[20]; char gateway[20];
    //  char name_server[20]; char default_lease_time[10]; char max_lease_time[10]; int num;    char buf[255];             
   
   mysql_init(&conn); //초기화
    if((connection = mysql_real_connect(&conn, DB_HOST, DB_USER, DB_PASSWD, DB_NAME, 0, (char *)NULL, 0)) == NULL)
    {
        fprintf(stderr, "MYSQL CONNECTION ERROR: %s\n", mysql_error(&conn));
        exit(1);
    }
    //다시 config file을 설정하고 싶을 때
    if(argv[1] != NULL) {
        if(strcmp(argv[1], "-d") == 0)
        {
            if(mysql_query(connection, "DROP TABLE ip_list"))
            {
                fprintf(stderr, "초기화 실패: %s\n", mysql_error(&conn));
                return 1;
            }
            printf("테이블 초기화 완료\n");
            return 1;
        }
        else if(strcmp(argv[1], "-s") == 0)
        {
            if(mysql_query(connection, "SELECT * FROM ip_list where Assigned = 1"))
            {   
                fprintf(stderr, "ip조회 실패: %s/\n", mysql_error(&conn));
                return 1;
            }
            printf("----------현재 ip 할당 list입니다----------\n");
            printf("%- 3s\t%-15s\t%-15s\t%-15s\t%-15s\n", "순서", "ip주소", "MAC주소", "시작 시간", "종료 시간");
            result = mysql_store_result(connection);
            int i = 1;
            while((row = mysql_fetch_row(result)) != NULL)
            {
                printf("%- 3d\t%-15s\t%-15s\t%-15s\t%-15s\n", i, row[0], row[1], row[8], row[9]);     
                i++;
            }
            mysql_free_result(result);

        }
    } 
    else
    {

        //Config file이 존재할때
        if(mysql_query(connection, "CREATE TABLE ip_list(ip_addr VARCHAR(20) PRIMARY KEY NOT NULL, MAC_addr VARCHAR(20), network VARCHAR(20) NOT NULL, subnet VARCHAR(20) NOT NULL, gateway VARCHAR(20) NOT NULL, name_server VARCHAR(20) NOT NULL, default_lease_time VARCHAR(20) NOT NULL, max_lease_time VARCHAR(20) NOT NULL, start VARCHAR(20), end VARCHAR(20), Assigned INT DEFAULT 0)" ))
        {
            query = mysql_query(connection, "SELECT network FROM ip_list");
            result = mysql_store_result(connection);
            query = mysql_num_rows(result);
            row = mysql_fetch_row(result); 
            mysql_free_result(result);
            if(query == 0);
            else 
                fprintf(stderr, "이미 %s에 대한 네트워크 정보가 존재합니다. 새로운 네트워크로 설정 원할시 -d옵션으로 초기화 후 이용해주세요\n", row[0]);     
            exit(1);              
        }

        if(mysql_query(connection, "CREATE TABLE tmp(ip INT UNSIGNED PRIMARY KEY)"))
        {
            fprintf(stderr, "ERROR: %s", mysql_error(&conn));
            exit(1);
        }
InputNetwork:
        printf("서비스를 시작하기 전 네트워크 설정을 해주세요.\n네트워크 대역(ex: 100.20.30.0):");
        scanf("%s", network);    
        //fgets(network, 20, stdin);
        while(fgetc(stdin) != '\n');
        printf("IP 시작 주소(ex: 100.20.30.10):");
        //fgets(start_ip_addr, 20, stdin);
        scanf("%s", start_ip_addr);    
        while(fgetc(stdin) != '\n');
        printf("IP 끝 주소(ex: 100.20.30.50):");
        //fgets(end_ip_addr, 20, stdin);
        scanf("%s", end_ip_addr);    
        while(fgetc(stdin) != '\n');
        printf("SubNet Mask(ex: 255.255.255.0):");
        //fgets(subnet, 20, stdin);
        scanf("%s", subnet);    
        while(fgetc(stdin) != '\n');
        printf("GateWay(ex: 100.20.30.1):");
        scanf("%s", gateway);    
        //fgets(gateway, 20, stdin);
        while(fgetc(stdin) != '\n');
        printf("domain-name-server(ex: 8.8.8.8):");
        //fgets(name_server, 20, stdin);
        scanf("%s", name_server);    
        while(fgetc(stdin) != '\n');
        printf("default_lease_time:");
        //fgets(default_lease_time, 10, stdin);
        scanf("%s", default_lease_time);    
        while(fgetc(stdin) != '\n');
        printf("max_lesase_time:");
        // fgets(max_lease_time, 10, stdin);
        scanf("%s", max_lease_time);    
        while(fgetc(stdin) != '\n');
        mysql_free_result(result);
        int count = CountIp(start_ip_addr, end_ip_addr);    //몇개의 ip범위가 나오는지
        if(count <= 0)
        {
            printf("ip범위가 잘못되었습니다. 다시 설정해주세요\n");
            goto InputNetwork;
        }
        int z;
        for(z = 0; z <= count; z++)
        {
            if(z == 0)
            {
                sprintf(buf, "INSERT INTO ip_list(ip_addr, network, subnet, gateway, name_server, default_lease_time, max_lease_time, Assigned) VALUES" " ('%s', '%s','%s','%s','%s','%s','%s','%d')", start_ip_addr, network, subnet, gateway, name_server, default_lease_time, max_lease_time,0);
                if(mysql_query(connection, buf) != 0)
                {
                    fprintf(stderr, "MYSQL ERROR %s", mysql_error(&conn));
                    exit(1);
                }
                printf("Network 설정이 완료되었습니다\n");
            }
            else
            {
                char *select_query = "SELECT INET_NTOA(ip) FROM tmp ORDER BY ip DESC LIMIT 1";
                if(mysql_query(connection, select_query))
                {
                    fprintf(stderr, "SELECT ERROR: %s", mysql_error(&conn));
                    exit(1);
                }
                result = mysql_store_result(connection);
                row = mysql_fetch_row(result);
                mysql_free_result(result);
                buf[0] = '\0';

                sprintf(buf, "INSERT INTO ip_list(ip_addr, network, subnet, gateway, name_server, default_lease_time, max_lease_time, Assigned) VALUES" " ('%s', '%s','%s','%s','%s','%s','%s','%d')", row[0], network, subnet, gateway, name_server, default_lease_time, max_lease_time,0);
                if(mysql_query(connection, buf) != 0)
                {
                    fprintf(stderr, "SECOND INSERT ERROR:%s", mysql_error(&conn));
                    exit(1);
                }
            }

            char* string_query = "INSERT INTO tmp(ip) VALUES((SELECT INET_ATON(ip_addr) FROM ip_list ORDER BY INET_ATON(ip_addr) DESC LIMIT 1)+1) ";
            if(mysql_query(connection, string_query))
            {
                fprintf(stderr, "INSERT ERROR: %s", mysql_error(&conn));
                exit(1);
            }
        }
        char* delete_query = "DROP TABLE tmp";  
        if(mysql_query(connection, delete_query))
        {
            fprintf(stderr, "DROP ERROR: %s", mysql_error(&conn));
            exit(1);
        }
    }*/  
    return 0;
}


int CountIp(char * start, char * end)
{
    //몇개의 ip갯수를 할당할 수 있는지
    int i = 0; int h = 0;  char tmp[5];  char tmp2[5];
    for(; *start != '\0'; start++);
    for(;  *start != '.'; start--);
    for(++start; *start != '\0'; start++)
    {
        tmp[i] = *start;
        i++;
    }

    for(; *end != '\0'; end++);
    for(;  *end != '.'; end--);
    for(++end; *end != '\0'; end++)
    {
        tmp2[h] = *end;
        h++;
    }
    return atoi(tmp2)-atoi(tmp);
} 