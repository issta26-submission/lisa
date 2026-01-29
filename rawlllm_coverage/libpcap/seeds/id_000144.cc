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
//<ID> 144
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpcap_api_sequence() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t* alldevs = nullptr;

    // Enumerate devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        std::fprintf(stderr, "pcap_findalldevs failed: %s\n", errbuf);
        return -1;
    }
    if (alldevs == nullptr) {
        std::fprintf(stderr, "No devices found by pcap_findalldevs\n");
        return -2;
    }

    // Choose the first device
    pcap_if_t* dev = alldevs;
    const char* devname = dev->name;
    if (devname == nullptr) {
        std::fprintf(stderr, "First device has no name\n");
        pcap_freealldevs(alldevs);
        return -3;
    }

    // Open the device for live capture
    pcap_t* handle = pcap_open_live(devname, 65535, 1 /* promiscuous */, 1000 /* timeout ms */, errbuf);
    if (handle == nullptr) {
        std::fprintf(stderr, "pcap_open_live failed on %s: %s\n", devname, errbuf);
        pcap_freealldevs(alldevs);
        return -4;
    }

    // Compile a simple BPF filter ("ip") and apply it
    struct bpf_program filter;
    const char* filter_exp = "ip";
    // Use unknown netmask if we don't parse device addresses here
    bpf_u_int32 netmask = PCAP_NETMASK_UNKNOWN;
    if (pcap_compile(handle, &filter, filter_exp, 1 /* optimize */, netmask) == -1) {
        std::fprintf(stderr, "pcap_compile failed: %s\n", pcap_geterr(handle));
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -5;
    }
    if (pcap_setfilter(handle, &filter) == -1) {
        std::fprintf(stderr, "pcap_setfilter failed: %s\n", pcap_geterr(handle));
        pcap_freecode(&filter);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -6;
    }

    // Query some properties
    int datalink = pcap_datalink(handle);
    int snaplen = pcap_snapshot(handle);
    (void)datalink; // used to demonstrate API usage
    (void)snaplen;

    // Try to capture a single packet (may be timeout or success)
    struct pcap_pkthdr* header = nullptr;
    const u_char* packet = nullptr;
    int rc = pcap_next_ex(handle, &header, &packet);
    if (rc == 1) {
        // successfully captured a packet; header and packet are valid
        // (we don't need to process the packet contents for this test)
    } else if (rc == 0) {
        // timeout elapsed; not treated as fatal here
    } else if (rc == -1) {
        std::fprintf(stderr, "pcap_next_ex error: %s\n", pcap_geterr(handle));
        // continue to cleanup and return failure
        pcap_freecode(&filter);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -7;
    } else if (rc == -2) {
        // No more packets (savefile EOF) - unlikely for live capture
    }

    // Cleanup
    pcap_freecode(&filter);
    pcap_close(handle);
    pcap_freealldevs(alldevs);

    // Success
    return 66;
}