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
//<ID> 668
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpcap_api_sequence() {
    // step 1: Initialization
    char errbuf[256];
    memset(errbuf, 0, sizeof(errbuf));
    FILE *in_tmp = tmpfile();
    unsigned char ghdr[24] = {
        0xd4,0xc3,0xb2,0xa1,
        0x02,0x00,
        0x04,0x00,
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0xff,0xff,0x00,0x00,
        0x01,0x00,0x00,0x00
    };
    unsigned char phdr[16] = {
        0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,
        0x04,0x00,0x00,0x00,
        0x04,0x00,0x00,0x00
    };
    unsigned char pdata[4] = {0x00, 0x01, 0x02, 0x03};
    fwrite(ghdr, 1, sizeof(ghdr), in_tmp);
    fwrite(phdr, 1, sizeof(phdr), in_tmp);
    fwrite(pdata, 1, sizeof(pdata), in_tmp);
    rewind(in_tmp);
    pcap_t *handle = pcap_fopen_offline(in_tmp, errbuf);

    // step 2: Configuration
    int can_rfmon = pcap_can_set_rfmon(handle);
    int curr_dlt = pcap_datalink(handle);
    int setd_ret = pcap_set_datalink(handle, curr_dlt);
    const char *dlt_desc = pcap_datalink_val_to_description_or_dlt(curr_dlt);
    int nonblock_ret = pcap_setnonblock(handle, 1, errbuf);

    // step 3: Operate
    struct pcap_pkthdr *hdrp = NULL;
    const u_char *pkt = NULL;
    int nex = pcap_next_ex(handle, &hdrp, &pkt);

    // step 4: Cleanup
    pcap_close(handle);
    // Do not fclose(in_tmp); pcap_close() closes the FILE* used by pcap_fopen_offline().
    puts("API sequence test completed successfully");
    (void)errbuf;
    (void)can_rfmon;
    (void)curr_dlt;
    (void)setd_ret;
    (void)dlt_desc;
    (void)nonblock_ret;
    (void)hdrp;
    (void)pkt;
    (void)nex;
    (void)in_tmp;
    return 66;
}