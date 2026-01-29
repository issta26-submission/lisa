#include <pcap/bluetooth.h>
#include <pcap/can_socketcan.h>
#include <pcap/compiler-tests.h>
#include <pcap/dlt.h>
#include <pcap/funcattrs.h>
#include <pcap/ipnet.h>
#include <pcap/nflog.h>
#include <pcap/pcap-inttypes.h>
#include <pcap/pcap.h>
#include <pcap/sll.h>
#include <pcap/socket.h>
#include <pcap/usb.h>
#include <pcap/vlan.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 646
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpcap_api_sequence() {
    // step 1: Initialize and prepare offline capture source
    char errbuf[256];
    memset(errbuf, 0, sizeof(errbuf));
    pcap_init(0, errbuf);
    FILE *offline_tmp = tmpfile();
    unsigned char ghdr[24] = {
        0xd4,0xc3,0xb2,0xa1,  /* magic (swapped) */
        0x02,0x00,            /* major version 2 */
        0x04,0x00,            /* minor version 4 */
        0x00,0x00,0x00,0x00,  /* thiszone */
        0x00,0x00,0x00,0x00,  /* sigfigs */
        0xff,0xff,0x00,0x00,  /* snaplen = 65535 */
        0x01,0x00,0x00,0x00   /* network = DLT_EN10MB */
    };
    unsigned char phdr[16] = {
        0x00,0x00,0x00,0x00,  /* ts_sec */
        0x00,0x00,0x00,0x00,  /* ts_usec */
        0x04,0x00,0x00,0x00,  /* incl_len = 4 */
        0x04,0x00,0x00,0x00   /* orig_len = 4 */
    };
    unsigned char pdata[4] = {0x00, 0x01, 0x02, 0x03};
    fwrite(ghdr, 1, sizeof(ghdr), offline_tmp);
    fwrite(phdr, 1, sizeof(phdr), offline_tmp);
    fwrite(pdata, 1, sizeof(pdata), offline_tmp);
    rewind(offline_tmp);
    pcap_t *offline = pcap_fopen_offline(offline_tmp, errbuf);

    // step 2: Configure a live handle for activation and buffer sizing
    pcap_t *live = pcap_create("any", errbuf);
    pcap_set_buffer_size(live, 64 * 1024);
    pcap_set_snaplen(live, 65535);
    pcap_set_promisc(live, 1);
    pcap_set_timeout(live, 1000);

    // step 3: Operate: activate live handle, inspect timestamp precision, read a packet, and open dumper
    int act_ret = pcap_activate(live);
    int tstamp_prec = pcap_get_tstamp_precision(offline);
    FILE *dump_tmp = tmpfile();
    pcap_dumper_t *dumper = pcap_dump_fopen(offline, dump_tmp);
    struct pcap_pkthdr *pkt_hdr = NULL;
    const u_char *pkt_data = NULL;
    int next_ret = pcap_next_ex(offline, &pkt_hdr, &pkt_data);

    // step 4: Validate and Cleanup
    pcap_dump((u_char *)dumper, pkt_hdr, pkt_data);
    pcap_dump_flush(dumper);
    pcap_dump_close(dumper);
    pcap_close(offline);
    /* Do not fclose(offline_tmp) here — pcap_fopen_offline/pсap_close may manage the FILE,
       closing it here can lead to a double free of the FILE stream. */
    pcap_close(live);
    puts("API sequence test completed successfully");
    (void)act_ret;
    (void)tstamp_prec;
    (void)next_ret;
    (void)errbuf;
    return 66;
}