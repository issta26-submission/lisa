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
//<ID> 34
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpcap_api_sequence() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t* alldevs = nullptr;

    // Find all devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        std::fprintf(stderr, "pcap_findalldevs failed: %s\n", errbuf);
        return 1;
    }

    if (!alldevs) {
        std::fprintf(stderr, "No devices found by pcap_findalldevs\n");
        return 1;
    }

    // Use the first device returned
    pcap_if_t* dev = alldevs;
    if (!dev->name) {
        std::fprintf(stderr, "First device has no name\n");
        pcap_freealldevs(alldevs);
        return 1;
    }
    const char* device_name = dev->name;

    // Open the device for live capture
    pcap_t* handle = pcap_open_live(device_name, BUFSIZ, 0 /*promisc*/, 1000 /*ms timeout*/, errbuf);
    if (!handle) {
        std::fprintf(stderr, "pcap_open_live failed for %s: %s\n", device_name, errbuf);
        pcap_freealldevs(alldevs);
        return 1;
    }

    // Optionally set non-blocking mode (demonstrates another API)
    if (pcap_setnonblock(handle, 1, errbuf) == -1) {
        std::fprintf(stderr, "pcap_setnonblock failed: %s\n", errbuf);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return 1;
    }

    // Compile a simple filter and set it
    struct bpf_program fp;
    const char* filter_exp = "ip"; // simple filter
    if (pcap_compile(handle, &fp, filter_exp, 1, PCAP_NETMASK_UNKNOWN) == -1) {
        std::fprintf(stderr, "pcap_compile failed: %s\n", pcap_geterr(handle));
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return 1;
    }

    if (pcap_setfilter(handle, &fp) == -1) {
        std::fprintf(stderr, "pcap_setfilter failed: %s\n", pcap_geterr(handle));
        pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return 1;
    }

    // Query link-layer type
    int dlt = pcap_datalink(handle);
    (void)dlt; // use cast to silence unused-variable warnings in contexts where it's not used

    // Try to read a single packet (will return 0 on timeout, 1 on packet, -1 on error, -2 on EOF)
    struct pcap_pkthdr* header = nullptr;
    const u_char* pkt_data = nullptr;
    int res = pcap_next_ex(handle, &header, &pkt_data);
    if (res == 1) {
        // Packet received; header and pkt_data valid
        // (We won't process the packet here; this demonstrates usage.)
    } else if (res == 0) {
        // Timeout expired - no packet available in the timeout period
    } else if (res == -1) {
        std::fprintf(stderr, "pcap_next_ex error: %s\n", pcap_geterr(handle));
        pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return 1;
    } else if (res == -2) {
        // EOF - capture handle read to end (not typical for live capture)
    }

    // Optionally retrieve capture statistics
    struct pcap_stat stats;
    if (pcap_stats(handle, &stats) != 0) {
        // If pcap_stats fails, it's non-fatal for this demonstration; just print a diagnostic.
        std::fprintf(stderr, "pcap_stats failed: %s\n", pcap_geterr(handle));
    } else {
        // stats.ps_recv, stats.ps_drop may be available depending on platform
        (void)stats;
    }

    // Cleanup
    pcap_freecode(&fp);
    pcap_close(handle);
    pcap_freealldevs(alldevs);

    // Indicate success as requested
    return 66;
}