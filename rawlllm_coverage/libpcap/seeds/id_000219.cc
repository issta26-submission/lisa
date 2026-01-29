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
//<ID> 219
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpcap_api_sequence() {
    pcap_if_t *alldevs = nullptr;
    pcap_if_t *device = nullptr;
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, sizeof(errbuf));

    // 1) Find all devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        fprintf(stderr, "pcap_findalldevs failed: %s\n", errbuf);
        return -1;
    }

    if (alldevs == nullptr) {
        // No devices found
        pcap_freealldevs(alldevs);
        return -2;
    }

    // Pick the first device for this test
    device = alldevs;

    // 2) Lookup network/netmask (optional, used for compiling filter)
    bpf_u_int32 netp = 0, mask = 0;
    if (pcap_lookupnet(device->name, &netp, &mask, errbuf) == -1) {
        // If lookup fails, use PCAP_NETMASK_UNKNOWN
#ifdef PCAP_NETMASK_UNKNOWN
        mask = PCAP_NETMASK_UNKNOWN;
#else
        mask = 0xFFFFFFFF;
#endif
    }

    // 3) Open device for live capture
    pcap_t *handle = pcap_open_live(device->name,            // device name
                                    65536,                  // snaplen
                                    1,                      // promisc
                                    1000,                   // timeout ms
                                    errbuf);
    if (handle == nullptr) {
        fprintf(stderr, "pcap_open_live failed: %s\n", errbuf);
        pcap_freealldevs(alldevs);
        return -3;
    }

    // 4) Compile a simple filter (e.g., capture only TCP)
    struct bpf_program fp;
    const char *filter_exp = "tcp";
    if (pcap_compile(handle, &fp, filter_exp, 1, mask) == -1) {
        fprintf(stderr, "pcap_compile failed: %s\n", pcap_geterr(handle));
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -4;
    }

    // 5) Set the compiled filter
    if (pcap_setfilter(handle, &fp) == -1) {
        fprintf(stderr, "pcap_setfilter failed: %s\n", pcap_geterr(handle));
        pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -5;
    }

    // 6) Retrieve a single packet (demonstrate pcap_next_ex usage)
    struct pcap_pkthdr *header = nullptr;
    const u_char *pkt_data = nullptr;
    int res = pcap_next_ex(handle, &header, &pkt_data);
    if (res == 1) {
        // Successfully captured a packet; use header->caplen / header->len if needed
        // Minimal safe use to avoid unused-variable warnings:
        (void) header;
        (void) pkt_data;
    } else if (res == 0) {
        // Timeout expired, no packet captured in the interval
    } else if (res == -1) {
        fprintf(stderr, "pcap_next_ex error: %s\n", pcap_geterr(handle));
        // continue to cleanup
    } else if (res == -2) {
        // EOF (offline reading exhausted), unlikely for live capture
    }

    // 7) Clean up
    pcap_freecode(&fp);
    pcap_close(handle);
    pcap_freealldevs(alldevs);

    // Success: return the required value
    return 66;
}