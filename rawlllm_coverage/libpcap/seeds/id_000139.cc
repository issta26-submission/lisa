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
//<ID> 139
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpcap_api_sequence() {
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));

    // 1) Find all devices and pick the first one
    pcap_if_t* alldevs = nullptr;
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        std::fprintf(stderr, "pcap_findalldevs failed: %s\n", errbuf);
        return -1;
    }
    if (!alldevs) {
        std::fprintf(stderr, "No devices found by pcap_findalldevs\n");
        return -1;
    }
    const char* device = alldevs->name;
    if (!device) {
        std::fprintf(stderr, "Selected device name is null\n");
        pcap_freealldevs(alldevs);
        return -1;
    }

    // 2) Open the device for live capture
    int snaplen = 65535;
    int promisc = 1;        // promiscuous mode
    int to_ms = 1000;       // read timeout in milliseconds
    pcap_t* handle = pcap_open_live(device, snaplen, promisc, to_ms, errbuf);
    if (!handle) {
        std::fprintf(stderr, "pcap_open_live failed: %s\n", errbuf);
        pcap_freealldevs(alldevs);
        return -1;
    }

    // 3) Compile and set a simple filter
    struct bpf_program fp;
    const char* filter_expr = "ip";
    bpf_u_int32 netmask = PCAP_NETMASK_UNKNOWN; // unknown netmask for compilation
    if (pcap_compile(handle, &fp, filter_expr, 1 /*optimize*/, netmask) == -1) {
        std::fprintf(stderr, "pcap_compile failed: %s\n", pcap_geterr(handle));
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -1;
    }
    if (pcap_setfilter(handle, &fp) == -1) {
        std::fprintf(stderr, "pcap_setfilter failed: %s\n", pcap_geterr(handle));
        pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -1;
    }
    // Free compiled code once installed
    pcap_freecode(&fp);

    // 4) Capture a single packet (non-blocking behavior depends on timeout)
    struct pcap_pkthdr* header = nullptr;
    const u_char* packet_data = nullptr;
    int rc = pcap_next_ex(handle, &header, &packet_data);
    if (rc == 1) {
        // packet captured successfully; header and packet_data are valid
        std::fprintf(stderr, "Captured one packet: len=%u caplen=%u\n",
                     header->len, header->caplen);
    } else if (rc == 0) {
        // timeout elapsed
        std::fprintf(stderr, "pcap_next_ex timed out (no packet captured within timeout)\n");
    } else if (rc == -1) {
        // error
        std::fprintf(stderr, "pcap_next_ex error: %s\n", pcap_geterr(handle));
    } else if (rc == -2) {
        // EOF - savefile has no more packets
        std::fprintf(stderr, "pcap_next_ex returned EOF (-2)\n");
    }

    // 5) Retrieve statistics (may fail for some capture types)
    struct pcap_stat stats;
    if (pcap_stats(handle, &stats) == 0) {
        std::fprintf(stderr, "pcap stats: ps_recv=%u ps_drop=%u ps_ifdrop=%u\n",
                     stats.ps_recv, stats.ps_drop, stats.ps_ifdrop);
    } else {
        // Some capture mechanisms don't support stats; log error string
        std::fprintf(stderr, "pcap_stats failed: %s\n", pcap_geterr(handle));
    }

    // 6) Query data-link type
    int dlt = pcap_datalink(handle);
    std::fprintf(stderr, "Data-link type: %d\n", dlt);

    // 7) Clean up
    pcap_close(handle);
    pcap_freealldevs(alldevs);

    // Success: return 66 as requested
    return 66;
}