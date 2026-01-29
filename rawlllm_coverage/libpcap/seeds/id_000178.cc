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
//<ID> 178
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpcap_api_sequence() {
    char errbuf[PCAP_ERRBUF_SIZE];

    // 1) Find all devices and pick the first one
    pcap_if_t *alldevs = nullptr;
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // failed to find devices
        return -1;
    }
    if (alldevs == nullptr) {
        // no devices found
        return -2;
    }
    pcap_if_t *dev = alldevs;
    if (dev->name == nullptr) {
        pcap_freealldevs(alldevs);
        return -3;
    }
    const char *devname = dev->name;

    // 2) Open the device for live capture
    pcap_t *handle = pcap_open_live(devname, 65535, 0 /*promisc*/, 1000 /*ms timeout*/, errbuf);
    if (handle == nullptr) {
        pcap_freealldevs(alldevs);
        return -4;
    }

    // 3) Compile and set a simple filter ("ip")
    struct bpf_program fp;
    const char *filter_exp = "ip";
    // Use PCAP_NETMASK_UNKNOWN when we don't know the netmask
    bpf_u_int32 netmask = PCAP_NETMASK_UNKNOWN;
    if (pcap_compile(handle, &fp, filter_exp, 1 /*optimize*/, netmask) == -1) {
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -5;
    }
    if (pcap_setfilter(handle, &fp) == -1) {
        pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -6;
    }

    // 4) Try to capture a single packet (non-blocking behavior controlled by timeout passed to pcap_open_live)
    struct pcap_pkthdr *header;
    const u_char *packet_data;
    int res = pcap_next_ex(handle, &header, &packet_data);
    if (res == 0) {
        // timeout expired with no packet; this is not a fatal error for this test
    } else if (res == 1) {
        // packet successfully captured; header and packet_data can be inspected if desired
        (void)header;
        (void)packet_data;
    } else if (res == -1) {
        // error
        pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -7;
    } else if (res == -2) {
        // EOF - only possible for savefiles, treat as non-fatal here
    }

    // 5) Retrieve capture statistics (may fail on some platforms; ignore non-fatal)
    struct pcap_stat stats;
    if (pcap_stats(handle, &stats) == 0) {
        (void)stats.ps_recv;
        (void)stats.ps_drop;
        (void)stats.ps_ifdrop;
    }

    // 6) Query some properties
    int dlt = pcap_datalink(handle);
    (void)dlt;
    int snaplen = pcap_snapshot(handle);
    (void)snaplen;

    // 7) Clean up
    pcap_freecode(&fp);
    pcap_close(handle);
    pcap_freealldevs(alldevs);

    // Success
    return 66;
}