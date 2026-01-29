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
//<ID> 0
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpcap_api_sequence() {
#if __has_include(<pcap/pcap.h>)
#elif __has_include(<pcap.h>)
#else
#error "pcap header not found"
#endif
    #include <cstdio>
    #include <cstring>
    #include <cstdlib>

    char errbuf[PCAP_ERRBUF_SIZE];
    std::memset(errbuf, 0, sizeof(errbuf));

    pcap_if_t *alldevs = nullptr;
    pcap_t *handle = nullptr;
    bool opened_live = false;
    bpf_u_int32 net = 0;
    bpf_u_int32 netmask = 0;
    struct bpf_program fp;
    bool compiled = false;

    // Find all devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        std::fprintf(stderr, "pcap_findalldevs failed: %s\n", errbuf);
        // No devices and failed -> try to continue with a dead handle fallback later
        alldevs = nullptr;
    }

    const char *devname = nullptr;
    if (alldevs != nullptr) {
        devname = alldevs->name; // choose first device
    }

    // Try to open live on the device if we have one
    if (devname) {
        std::memset(errbuf, 0, sizeof(errbuf));
        handle = pcap_open_live(devname, 65535, 0 /* non-promiscuous */, 1000 /* ms timeout */, errbuf);
        if (handle == nullptr) {
            std::fprintf(stderr, "pcap_open_live(%s) failed: %s\n", devname, errbuf);
        } else {
            opened_live = true;
            // Try to get network & mask for filter compilation
            if (pcap_lookupnet(devname, &net, &netmask, errbuf) == -1) {
                // Non-fatal: fallback to 0 if lookup fails
                std::fprintf(stderr, "pcap_lookupnet(%s) failed, continuing with netmask=0: %s\n", devname, errbuf);
                net = 0;
                netmask = 0;
            }
        }
    }

    // If we couldn't open a live device, create a dead handle to allow compile/setfilter tests
    if (handle == nullptr) {
        // Choose a generic linktype and snaplen for a dead handle
        int linktype = DLT_RAW; // raw IP; may be changed depending on platform
        handle = pcap_open_dead(linktype, 65535);
        if (handle == nullptr) {
            std::fprintf(stderr, "pcap_open_dead failed\n");
            if (alldevs) pcap_freealldevs(alldevs);
            return -1;
        }
        opened_live = false; // explicit
        netmask = 0;
    }

    // Query datalink type and name (demonstrates pcap_datalink and pcap_datalink_val_to_name)
    int dlt = pcap_datalink(handle);
    const char *dlt_name = pcap_datalink_val_to_name(dlt);
    if (dlt_name) {
        // Optionally use the name; here we just print to stderr for diagnostics
        std::fprintf(stderr, "datalink type: %d (%s)\n", dlt, dlt_name);
    } else {
        std::fprintf(stderr, "datalink type: %d (name unknown)\n", dlt);
    }

    // Compile a simple filter
    const char *filter_expr = "tcp or udp";
    if (pcap_compile(handle, &fp, filter_expr, 1 /* optimize */, netmask) == -1) {
        std::fprintf(stderr, "pcap_compile failed: %s\n", pcap_geterr(handle));
        pcap_close(handle);
        if (alldevs) pcap_freealldevs(alldevs);
        return -1;
    }
    compiled = true;

    // Set the compiled filter
    if (pcap_setfilter(handle, &fp) == -1) {
        std::fprintf(stderr, "pcap_setfilter failed: %s\n", pcap_geterr(handle));
        pcap_freecode(&fp);
        pcap_close(handle);
        if (alldevs) pcap_freealldevs(alldevs);
        return -1;
    }

    // If we opened a live handle, attempt to capture one packet using pcap_next_ex
    if (opened_live) {
        struct pcap_pkthdr *header = nullptr;
        const u_char *packet = nullptr;
        int res = pcap_next_ex(handle, &header, &packet);
        if (res == 1) {
            // Got a packet; we can inspect header->len etc.
            std::fprintf(stderr, "Captured one packet: caplen=%u, len=%u\n", header->caplen, header->len);
        } else if (res == 0) {
            // Timeout expired
            std::fprintf(stderr, "pcap_next_ex: timeout expired (no packet captured)\n");
        } else if (res == -1) {
            std::fprintf(stderr, "pcap_next_ex error: %s\n", pcap_geterr(handle));
        } else if (res == -2) {
            std::fprintf(stderr, "pcap_next_ex: EOF reached\n");
        }
    } else {
        // Not live: demonstrate that calling pcap_next_ex would not make sense on a dead handle.
        std::fprintf(stderr, "Handle is dead (pcap_open_dead); skipping packet capture\n");
    }

    // Retrieve statistics (may fail on some platforms, handle non-fatal)
    struct pcap_stat stats;
    if (pcap_stats(handle, &stats) == 0) {
        std::fprintf(stderr, "pcap_stats: ps_recv=%u, ps_drop=%u, ps_ifdrop=%u\n",
                     (unsigned)stats.ps_recv, (unsigned)stats.ps_drop, (unsigned)stats.ps_ifdrop);
    } else {
        std::fprintf(stderr, "pcap_stats not available or failed: %s\n", pcap_geterr(handle));
    }

    // Clean up resources
    if (compiled) pcap_freecode(&fp);
    pcap_close(handle);
    if (alldevs) pcap_freealldevs(alldevs);

    // If we reached here without returns above, consider the sequence successful
    return 66;
}