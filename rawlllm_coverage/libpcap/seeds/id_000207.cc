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
//<ID> 207
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

// A self-contained function that demonstrates a sequence of libpcap API calls.
// Returns 66 on success, non-66 on failure.
int test_libpcap_api_sequence() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t* alldevs = nullptr;
    pcap_if_t* dev = nullptr;
    pcap_t* handle = nullptr;
    struct pcap_pkthdr* header = nullptr;
    const u_char* packet = nullptr;
    struct bpf_program fcode;   // compiled filter
    bool fcode_compiled = false;

    // 1) Find all devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // If findalldevs fails, try the legacy lookup (best effort).
        pcap_if_t* tmp = nullptr;
        const char* lookup = pcap_lookupdev(errbuf);
        if (lookup == nullptr) {
            // Couldn't find any device
            return -1;
        }
        // Create a tiny fake pcap_if_t to use the lookup result (no need to free)
        // but to keep logic simple, we open the device directly.
        dev = nullptr;
        handle = pcap_open_live(lookup, 65535, 1, 1000, errbuf);
        if (!handle) return -2;
    } else {
        // Use the first device found (if any)
        if (alldevs == nullptr) {
            // no devices found
            return -3;
        }
        dev = alldevs;
        // 2) Open the device for live capture
        handle = pcap_open_live(dev->name, 65535, 1, 1000, errbuf);
        if (!handle) {
            pcap_freealldevs(alldevs);
            return -4;
        }
    }

    // At this point handle should be valid
    if (!handle) {
        // defensive check
        if (alldevs) pcap_freealldevs(alldevs);
        return -5;
    }

    // 3) Query datalink type just as an API usage example
    int dlt = pcap_datalink(handle);
    (void)dlt; // not used further, just demonstrating call

    // 4) Compile a simple BPF filter ("ip")
#if defined(PCAP_NETMASK_UNKNOWN)
    bpf_u_int32 netmask = PCAP_NETMASK_UNKNOWN;
#else
    bpf_u_int32 netmask = 0xFFFFFFFF;
#endif
    const char* filter_exp = "ip";
    if (pcap_compile(handle, &fcode, filter_exp, 1, netmask) == -1) {
        // compilation failed; cleanup and return error
        pcap_close(handle);
        if (alldevs) pcap_freealldevs(alldevs);
        return -6;
    }
    fcode_compiled = true;

    // 5) Set the compiled filter
    if (pcap_setfilter(handle, &fcode) == -1) {
        if (fcode_compiled) pcap_freecode(&fcode);
        pcap_close(handle);
        if (alldevs) pcap_freealldevs(alldevs);
        return -7;
    }

    // 6) Try to fetch one packet with pcap_next_ex (non-blocking behavior depends on timeout)
    int ret = pcap_next_ex(handle, &header, &packet);
    if (ret == 1) {
        // A packet was read; we can inspect header->caplen and header->len safely.
        // For this test function we don't need to process the packet contents.
        (void)packet;
        (void)header;
    } else if (ret == 0) {
        // Timeout expired; still considered a successful sequence of API calls
    } else if (ret == -1) {
        // Error during capture
        if (fcode_compiled) pcap_freecode(&fcode);
        pcap_close(handle);
        if (alldevs) pcap_freealldevs(alldevs);
        return -8;
    } else if (ret == -2) {
        // EOF: happens when reading from a savefile; treat as non-fatal for API usage test
    }

    // 7) Optionally retrieve stats (may fail on some capture types)
    struct pcap_stat stats;
    if (pcap_stats(handle, &stats) == 0) {
        // Stats successfully retrieved; we don't need to use them further here.
        (void)stats.ps_recv;
        (void)stats.ps_drop;
#if defined(HAVE_PCAP_STAT_PS_IFDROP)
        (void)stats.ps_ifdrop;
#endif
    }

    // 8) Demonstrate ability to break a loop (we won't actually start a loop here),
    // but show the API exists by calling pcap_breakloop if desired. Not needed in this simple test.

    // Cleanup resources
    if (fcode_compiled) pcap_freecode(&fcode);
    pcap_close(handle);
    if (alldevs) pcap_freealldevs(alldevs);

    // Success
    return 66;
}