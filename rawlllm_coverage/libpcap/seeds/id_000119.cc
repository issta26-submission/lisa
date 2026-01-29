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
//<ID> 119
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
    // Find all devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // error finding devices
        return -1;
    }
    if (alldevs == nullptr) {
        // no devices found
        return -2;
    }

    // Pick the first device
    pcap_if_t *dev = alldevs;
    const char *device_name = dev->name;
    if (device_name == nullptr) {
        pcap_freealldevs(alldevs);
        return -3;
    }

    // Get network and mask (non-fatal if it fails; we set to 0)
    bpf_u_int32 netp = 0, maskp = 0;
    if (pcap_lookupnet(device_name, &netp, &maskp, errbuf) == -1) {
        netp = 0;
        maskp = 0;
    }

    // Open the device for live capture
    pcap_t *handle = pcap_open_live(device_name, 65536, 1 /*promiscuous*/, 1000 /*ms timeout*/, errbuf);
    if (handle == nullptr) {
        pcap_freealldevs(alldevs);
        return -4;
    }

    // Compile a simple filter (e.g., capture only TCP)
    struct bpf_program fp;
    const char *filter_exp = "tcp";
    if (pcap_compile(handle, &fp, filter_exp, 1 /*optimize*/, maskp) == -1) {
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -5;
    }

    // Apply the compiled filter
    if (pcap_setfilter(handle, &fp) == -1) {
        pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -6;
    }

    // Attempt to read a single packet (pcap_next_ex: 1=packet, 0=timeout, -1=error, -2=EOF)
    struct pcap_pkthdr *header = nullptr;
    const u_char *packet = nullptr;
    int res = pcap_next_ex(handle, &header, &packet);
    if (res == -1) {
        // capture error
        pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -7;
    }
    // res == 0 (timeout) or res == 1 (got packet) or res == -2 (EOF) are acceptable for this sequence

    // Query data-link type as another API usage
    int linktype = pcap_datalink(handle);
    (void)linktype; // silence unused-variable warnings; value may be used for further logic

    // Clean up
    pcap_freecode(&fp);
    pcap_close(handle);
    pcap_freealldevs(alldevs);

    // If we reached here, the API sequence succeeded
    return 66;
}