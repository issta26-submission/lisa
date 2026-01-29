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
//<ID> 141
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpcap_api_sequence() {
    // include allowed inside function so the snippet is self-contained
    #include <pcap/pcap.h>

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = nullptr;
    pcap_t *handle = nullptr;
    struct bpf_program filter = {};
    struct pcap_pkthdr *header = nullptr;
    const u_char *packet_data = nullptr;

    // 1) Find all devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // failure to enumerate devices
        return -1;
    }

    if (alldevs == nullptr) {
        // no devices found
        return -2;
    }

    // 2) Open first device for live capture
    const char *devname = alldevs->name;
    handle = pcap_open_live(devname, 65536 /*snaplen*/, 1 /*promisc*/, 1000 /*to_ms*/, errbuf);
    if (handle == nullptr) {
        pcap_freealldevs(alldevs);
        return -3;
    }

    // 3) Compile a simple filter (tcp)
    const char *filter_expr = "tcp";
    if (pcap_compile(handle, &filter, filter_expr, 1 /*optimize*/, 0 /*netmask*/) == -1) {
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -4;
    }

    // 4) Set the compiled filter
    if (pcap_setfilter(handle, &filter) == -1) {
        pcap_freecode(&filter);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -5;
    }

    // 5) Try to fetch a single packet (non-blocking behavior depends on timeout and traffic)
    int nex_res = pcap_next_ex(handle, &header, &packet_data);
    // nex_res: 1 = packet read, 0 = timeout, -1 = error, -2 = EOF
    // We treat all non-fatal outcomes as still a successful sequence of API usage.

    // 6) Retrieve statistics (optional, may fail on some platforms)
    struct pcap_stat stats;
    (void)pcap_stats(handle, &stats); // ignore return, just exercising the API

    // 7) Cleanup
    pcap_freecode(&filter);
    pcap_close(handle);
    pcap_freealldevs(alldevs);

    // If we reached here, the API sequence succeeded
    return 66;
}