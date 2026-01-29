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
//<ID> 181
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpcap_api_sequence() {
    char errbuf[PCAP_ERRBUF_SIZE];

    // Find all devices
    pcap_if_t *alldevs = nullptr;
    if (pcap_findalldevs(&alldevs, errbuf) != 0) {
        std::fprintf(stderr, "pcap_findalldevs failed: %s\n", errbuf);
        return -1;
    }
    if (alldevs == nullptr) {
        std::fprintf(stderr, "No devices found by pcap_findalldevs\n");
        // nothing to free in this case
        return -2;
    }

    // Choose the first device
    pcap_if_t *dev = alldevs;
    const char *devname = dev->name;
    if (devname == nullptr) {
        std::fprintf(stderr, "First device has no name\n");
        pcap_freealldevs(alldevs);
        return -3;
    }

    // Look up network and mask for the device (optional; continue on failure)
    bpf_u_int32 net = 0;
    bpf_u_int32 mask = 0;
    if (pcap_lookupnet(devname, &net, &mask, errbuf) == -1) {
        std::fprintf(stderr, "pcap_lookupnet failed for %s: %s\n", devname, errbuf);
        // Continue: net/mask remain 0 which is acceptable for compiling a filter
    }

    // Open the device for live capture
    pcap_t *handle = pcap_open_live(devname, BUFSIZ, 1 /* promiscuous */, 1000 /* ms timeout */, errbuf);
    if (handle == nullptr) {
        std::fprintf(stderr, "pcap_open_live failed on %s: %s\n", devname, errbuf);
        pcap_freealldevs(alldevs);
        return -4;
    }

    // Compile a simple filter (e.g., capture only TCP)
    struct bpf_program fp;
    const char *filter_exp = "tcp";
    if (pcap_compile(handle, &fp, filter_exp, 1 /* optimize */, net) == -1) {
        std::fprintf(stderr, "pcap_compile failed: %s\n", pcap_geterr(handle));
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -5;
    }

    // Set the compiled filter
    if (pcap_setfilter(handle, &fp) == -1) {
        std::fprintf(stderr, "pcap_setfilter failed: %s\n", pcap_geterr(handle));
        pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -6;
    }

    // Free compiled filter (no longer needed after set)
    pcap_freecode(&fp);

    // Attempt to fetch one packet with a timeout (non-blocking beyond the timeout)
    struct pcap_pkthdr *header = nullptr;
    const u_char *packet = nullptr;
    int res = pcap_next_ex(handle, &header, &packet);
    if (res == 1) {
        // Got a packet
        std::fprintf(stdout, "Captured one packet: length=%u, caplen=%u\n", header->len, header->caplen);
    } else if (res == 0) {
        // Timeout expired
        std::fprintf(stdout, "pcap_next_ex timeout expired, no packet captured within timeout\n");
    } else if (res == -1) {
        // Error
        std::fprintf(stderr, "pcap_next_ex error: %s\n", pcap_geterr(handle));
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -7;
    } else if (res == -2) {
        // EOF (offline capture ended)
        std::fprintf(stdout, "pcap_next_ex returned EOF (offline capture ended)\n");
    }

    // Optionally query properties
    int datalink = pcap_datalink(handle);
    std::fprintf(stdout, "Data link type: %d\n", datalink);

    // Close and free resources
    pcap_close(handle);
    pcap_freealldevs(alldevs);

    // Indicate success per request
    return 66;
}