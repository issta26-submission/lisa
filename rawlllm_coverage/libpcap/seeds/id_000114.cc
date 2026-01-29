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
//<ID> 114
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
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // failure finding devices
        return 1;
    }

    if (!alldevs) {
        pcap_freealldevs(alldevs);
        return 2;
    }

    // Pick the first device
    const char *device = alldevs->name;
    bpf_u_int32 netp = 0, mask = 0;

    // Lookup network and mask (best effort)
    if (pcap_lookupnet(device, &netp, &mask, errbuf) == -1) {
        // continue with zeroed net/mask if lookup fails
        netp = 0;
        mask = 0;
    }

    // Open the device for live capture
    pcap_t *handle = pcap_open_live(device, 65536, 1, 1000, errbuf);
    if (!handle) {
        pcap_freealldevs(alldevs);
        return 3;
    }

    // Get data-link type (unused here, just exercising API)
    int dlt = pcap_datalink(handle);
    (void)dlt;

    // Compile and set a simple filter
    struct bpf_program fp;
    const char *filter_exp = "tcp port 80";
    if (pcap_compile(handle, &fp, filter_exp, 1, mask) == -1) {
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return 4;
    }

    if (pcap_setfilter(handle, &fp) == -1) {
        pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return 5;
    }

    // Set non-blocking mode (exercise API). If it fails, clean up and return.
    if (pcap_setnonblock(handle, 1, errbuf) == -1) {
        pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return 6;
    }

    // Read up to a few packets using pcap_next_ex (no callback needed)
    const u_char *pkt_data = nullptr;
    struct pcap_pkthdr *header = nullptr;
    int received = 0;
    for (int i = 0; i < 5; ++i) {
        int res = pcap_next_ex(handle, &header, &pkt_data);
        if (res == 1) {
            // packet received; minimal processing (count)
            ++received;
        } else if (res == 0) {
            // timeout expired (non-blocking or read timeout); continue
            continue;
        } else {
            // res == -1 -> error, res == -2 -> EOF, break out
            break;
        }
    }
    (void)received; // avoid unused-variable warnings

    // Retrieve stats (best-effort)
    struct pcap_stat stats;
    if (pcap_stats(handle, &stats) == 0) {
        // stats retrieved successfully; not used further here
    }

    // Cleanup
    pcap_freecode(&fp);
    // restore blocking mode (best-effort)
    pcap_setnonblock(handle, 0, errbuf);
    pcap_close(handle);
    pcap_freealldevs(alldevs);

    // Indicate success as requested
    return 66;
}