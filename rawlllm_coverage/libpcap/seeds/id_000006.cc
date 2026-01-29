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
//<ID> 6
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
#if defined(__has_include)
  #if __has_include(<pcap/pcap.h>)
    #include <pcap/pcap.h>
  #elif __has_include(<pcap.h>)
    #include <pcap.h>
  #else
    #error "pcap header not found"
  #endif
#else
  #include <pcap/pcap.h>
#endif


int test_libpcap_api_sequence() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = nullptr;
    pcap_t *handle = nullptr;
    struct bpf_program fp;
    bool compiled = false;

    // 1) Find all devices
    if (pcap_findalldevs(&alldevs, errbuf) != 0) {
        // failure to find devices
        return -1;
    }
    if (alldevs == nullptr) {
        // no devices found
        return -2;
    }

    // 2) Choose the first device
    pcap_if_t *dev = alldevs;
    if (dev->name == nullptr) {
        pcap_freealldevs(alldevs);
        return -3;
    }

    // 3) Open the device for live capture
    handle = pcap_open_live(dev->name, BUFSIZ, 1 /* promisc */, 1000 /* ms timeout */, errbuf);
    if (handle == nullptr) {
        pcap_freealldevs(alldevs);
        return -4;
    }

    // 4) Compile a simple filter ("tcp")
    const char *filter_expr = "tcp";
    if (pcap_compile(handle, &fp, filter_expr, 1 /* optimize */, PCAP_NETMASK_UNKNOWN) == -1) {
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -5;
    }
    compiled = true;

    // 5) Set the compiled filter
    if (pcap_setfilter(handle, &fp) == -1) {
        if (compiled) pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -6;
    }

    // 6) Try to fetch one packet using pcap_next_ex (synchronous)
    struct pcap_pkthdr *header = nullptr;
    const u_char *packet_data = nullptr;
    int attempts = 3;
    int result = 0;
    bool got_packet = false;
    for (int i = 0; i < attempts; ++i) {
        result = pcap_next_ex(handle, &header, &packet_data);
        if (result == 1) { // packet read
            got_packet = true;
            break;
        } else if (result == 0) {
            // timeout elapsed, try again
            continue;
        } else {
            // error (-1) or EOF (-2)
            break;
        }
    }
    // We do not require a packet to succeed overall; proceed to stats and cleanup even if none captured.

    // 7) Retrieve capture statistics
    struct pcap_stat stats;
    if (pcap_stats(handle, &stats) != 0) {
        // stats may not be available on all platforms; treat as non-fatal for this test but note the error
        // fall through to cleanup
        ;
    } else {
        // stats available; nothing further required here
        (void)stats.ps_recv; // avoid unused warning in some builds
    }

    // 8) Clean up
    if (compiled) {
        pcap_freecode(&fp);
        compiled = false;
    }
    if (handle) {
        pcap_close(handle);
        handle = nullptr;
    }
    if (alldevs) {
        pcap_freealldevs(alldevs);
        alldevs = nullptr;
    }

    // If we reached here, sequence of API calls completed correctly
    return 66;
}