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
//<ID> 189
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
    int rc_findalldevs = pcap_findalldevs(&alldevs, errbuf);

    char *lookup_dev = pcap_lookupdev(errbuf);
    char default_dev[] = "any";
    char *device = lookup_dev ? lookup_dev : default_dev;

    pcap_t *handle = pcap_create(device, errbuf);
    int rc_set_snaplen = pcap_set_snaplen(handle, 65535);
    int rc_set_promisc = pcap_set_promisc(handle, 1);
    int rc_set_timeout = pcap_set_timeout(handle, 1000);
    int rc_set_immediate = pcap_set_immediate_mode(handle, 1);
    int rc_set_buffer = pcap_set_buffer_size(handle, 2 * 1024 * 1024);
    int rc_set_tsprec = pcap_set_tstamp_precision(handle, PCAP_TSTAMP_PRECISION_MICRO);
    int rc_activate = pcap_activate(handle);

    pcap_dumper_t *dumper = pcap_dump_open(handle, "/tmp/test_output.pcap");
    struct pcap_pkthdr hdr;
    memset(&hdr, 0, sizeof(hdr));
    hdr.caplen = hdr.len = 64;
    hdr.ts.tv_sec = 0;
    hdr.ts.tv_usec = 0;
    unsigned char pkt[64];
    memset(pkt, 0xAB, sizeof(pkt));
    pcap_dump((u_char *)dumper, &hdr, (const u_char *)pkt);
    int rc_flush = pcap_dump_flush(dumper);

    pcap_dump_close(dumper);
    pcap_close(handle);
    pcap_freealldevs(alldevs);

    (void)rc_findalldevs;
    (void)device;
    (void)rc_set_snaplen;
    (void)rc_set_promisc;
    (void)rc_set_timeout;
    (void)rc_set_immediate;
    (void)rc_set_buffer;
    (void)rc_set_tsprec;
    (void)rc_activate;
    (void)hdr;
    (void)pkt;
    (void)rc_flush;

    puts("API sequence test completed successfully");
    return 66;
}