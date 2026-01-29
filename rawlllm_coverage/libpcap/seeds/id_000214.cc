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
//<ID> 214
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpcap_api_sequence() {
    char errbuf[PCAP_ERRBUF_SIZE];
    std::memset(errbuf, 0, sizeof(errbuf));

    pcap_if_t *alldevs = nullptr;
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // failed to find devices
        return 1;
    }

    if (alldevs == nullptr) {
        // no devices found
        return 2;
    }

    // Choose the first device found
    const char *devname = alldevs->name;
    if (devname == nullptr) {
        pcap_freealldevs(alldevs);
        return 3;
    }

    // Open the device for live capture (non-promiscuous to avoid needing root in some environments)
    pcap_t *handle = pcap_open_live(devname, BUFSIZ, 0 /*promisc*/, 1000 /*ms timeout*/, errbuf);
    if (handle == nullptr) {
        pcap_freealldevs(alldevs);
        return 4;
    }

    // Prepare a simple filter (capture only IP packets)
    struct bpf_program fp;
    const char *filter_exp = "ip";
    if (pcap_compile(handle, &fp, filter_exp, 1 /*optimize*/, PCAP_NETMASK_UNKNOWN) == -1) {
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return 5;
    }

    if (pcap_setfilter(handle, &fp) == -1) {
        pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return 6;
    }

    // Try to read up to a few packets (respecting timeout)
    struct pcap_pkthdr *pkt_header = nullptr;
    const u_char *pkt_data = nullptr;
    int attempts = 0;
    const int max_attempts = 3;
    bool got_packet = false;
    while (attempts < max_attempts) {
        int res = pcap_next_ex(handle, &pkt_header, &pkt_data);
        if (res == 1) {
            // Successfully read a packet
            if (pkt_header != nullptr && pkt_data != nullptr) {
                got_packet = true;
                break;
            }
        } else if (res == 0) {
            // Timeout expired, try again
            attempts++;
            continue;
        } else if (res == -1) {
            // Error occurred
            // Use pcap_geterr(handle) if needed; just cleanup and return error
            pcap_freecode(&fp);
            pcap_close(handle);
            pcap_freealldevs(alldevs);
            return 7;
        } else if (res == -2) {
            // EOF (offline capture), unlikely for live capture
            break;
        }
    }

    // Clean up resources
    pcap_freecode(&fp);
    pcap_close(handle);
    pcap_freealldevs(alldevs);

    if (!got_packet) {
        // No packet captured within attempts
        return 8;
    }

    // If we reach here, the sequence of libpcap APIs executed successfully
    return 66;
}