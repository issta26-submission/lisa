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
//<ID> 171
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpcap_api_sequence() {

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = nullptr;

    /* Discover devices */
    if (pcap_findalldevs(&alldevs, errbuf) != 0) {
        /* failure discovering devices */
        return -1;
    }
    if (alldevs == nullptr) {
        /* no devices found */
        return -2;
    }

    /* Use the first discovered device */
    pcap_if_t *dev = alldevs;

    /* Open the device for live capture */
    pcap_t *handle = pcap_open_live(dev->name,            /* device name */
                                   65535,                /* snaplen */
                                   1,                    /* promisc */
                                   1000,                 /* to_ms */
                                   errbuf);              /* errbuf */
    if (!handle) {
        pcap_freealldevs(alldevs);
        return -3;
    }

    /* Compile and set a simple filter ("ip") */
    struct bpf_program fp{};
    const char *filter_expr = "ip";
    bpf_u_int32 netmask = 0xFFFFFF00; /* reasonable default if we don't compute it */

    if (pcap_compile(handle, &fp, filter_expr, 1, netmask) != 0) {
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -4;
    }

    if (pcap_setfilter(handle, &fp) != 0) {
        pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -5;
    }

    /* Attempt to fetch a single packet (pcap_next_ex returns:
       1 = packet, 0 = timeout, -1 = error, -2 = eof) */
    struct pcap_pkthdr *header = nullptr;
    const u_char *packet = nullptr;
    int res = pcap_next_ex(handle, &header, &packet);

    /* Cleanup */
    pcap_freecode(&fp);
    pcap_close(handle);
    pcap_freealldevs(alldevs);

    /* Consider success if we either received a packet (1) or timed out (0)
       as the API sequence itself succeeded. */
    if (res == 1 || res == 0) {
        return 66;
    }

    return -6;
}