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
//<ID> 203
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpcap_api_sequence() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = nullptr;

    // Find all devices
    if (pcap_findalldevs(&alldevs, errbuf) != 0) {
        fprintf(stderr, "pcap_findalldevs failed: %s\n", errbuf);
        return -1;
    }

    if (alldevs == nullptr) {
        fprintf(stderr, "No devices found by pcap_findalldevs\n");
        return -2;
    }

    // Use the first device found
    pcap_if_t *dev = alldevs;
    const char *devname = dev->name;
    if (devname == nullptr) {
        fprintf(stderr, "Device name is null\n");
        pcap_freealldevs(alldevs);
        return -3;
    }

    // Try to open the device for live capture
    pcap_t *handle = pcap_open_live(devname, 65535, 0 /* non-promiscuous */, 1000 /* ms timeout */, errbuf);
    bool opened_live = true;
    if (handle == nullptr) {
        // If live open fails (e.g., permission), fall back to a "dead" handle for compilation/testing
        fprintf(stderr, "pcap_open_live failed for %s: %s\n", devname, errbuf);
        handle = pcap_open_dead(DLT_EN10MB, 65535);
        if (handle == nullptr) {
            fprintf(stderr, "pcap_open_dead failed\n");
            pcap_freealldevs(alldevs);
            return -4;
        }
        opened_live = false;
    }

    // Query snapshot length and link-layer type
    int snapshot = pcap_snapshot(handle);
    int linktype = pcap_datalink(handle);
    (void)snapshot; // silence unused warnings if not used further
    (void)linktype;

    // Compile a simple filter
    struct bpf_program fp;
    const char *filter_exp = "ip";
    if (pcap_compile(handle, &fp, filter_exp, 1 /* optimize */, PCAP_NETMASK_UNKNOWN) != 0) {
        fprintf(stderr, "pcap_compile failed: %s\n", pcap_geterr(handle));
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -5;
    }

    // Apply the filter only if we have a live handle
    if (opened_live) {
        if (pcap_setfilter(handle, &fp) != 0) {
            fprintf(stderr, "pcap_setfilter failed: %s\n", pcap_geterr(handle));
            pcap_freecode(&fp);
            pcap_close(handle);
            pcap_freealldevs(alldevs);
            return -6;
        }
    }

    // If live, attempt to capture a single packet (non-fatal if none or error)
    if (opened_live) {
        struct pcap_pkthdr *header = nullptr;
        const u_char *pkt_data = nullptr;
        int res = pcap_next_ex(handle, &header, &pkt_data);
        if (res == 1) {
            // Successfully captured one packet; header and pkt_data are valid
            // For test purposes, we won't inspect the packet contents further
            (void)header;
            (void)pkt_data;
        } else if (res == 0) {
            // Timeout expired, no packet captured — acceptable for a test
        } else if (res == -1) {
            fprintf(stderr, "pcap_next_ex error: %s\n", pcap_geterr(handle));
            // Continue; not considered a fatal failure for the API sequence test
        } else if (res == -2) {
            // EOF (offline capture ended) — not expected for live, but handle gracefully
        }

        // Retrieve stats if available (may fail on some platforms)
        struct pcap_stat stats;
        if (pcap_stats(handle, &stats) == 0) {
            // Use stats fields to ensure API was exercised
            (void)stats.ps_recv;
            (void)stats.ps_drop;
            (void)stats.ps_ifdrop;
        }
    }

    // Clean up
    pcap_freecode(&fp);
    pcap_close(handle);
    pcap_freealldevs(alldevs);

    // Indicate success as requested
    return 66;
}