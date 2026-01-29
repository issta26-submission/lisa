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
//<ID> 699
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpcap_api_sequence() {
    char errbuf[256];
    memset(errbuf, 0, sizeof(errbuf));

    pcap_if_t *alldevs = NULL;
    int find_ret = pcap_findalldevs(&alldevs, errbuf);
    char *dev = pcap_lookupdev(errbuf);
    pcap_t *handle = pcap_create(dev, errbuf);

    int snap_ret = pcap_set_snaplen(handle, 65535);
    int promisc_ret = pcap_set_promisc(handle, 1);
    int proto_ret = pcap_set_protocol_linux(handle, 1);
    int timeout_ret = pcap_set_timeout(handle, 1000);
    int immediate_ret = pcap_set_immediate_mode(handle, 1);

    int act_ret = pcap_activate(handle);
    int nonblock_ret = pcap_setnonblock(handle, 1, errbuf);

    int dlt = pcap_datalink(handle);
    const char *dlt_name = pcap_datalink_val_to_name(dlt);

    struct pcap_pkthdr header;
    const u_char *packet = pcap_next(handle, &header);

    pcap_close(handle);
    pcap_freealldevs(alldevs);

    puts("API sequence test completed successfully");

    (void)find_ret;
    (void)dev;
    (void)snap_ret;
    (void)promisc_ret;
    (void)proto_ret;
    (void)timeout_ret;
    (void)immediate_ret;
    (void)act_ret;
    (void)nonblock_ret;
    (void)dlt;
    (void)dlt_name;
    (void)packet;
    (void)header;
    (void)errbuf;

    return 66;
}