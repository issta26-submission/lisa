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
//<ID> 220
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
        // failed to enumerate devices
        return 1;
    }

    // If no devices found, still consider the API sequence successful (no fatal error)
    if (alldevs == nullptr) {
        // nothing to open, clean up and succeed
        pcap_freealldevs(alldevs);
        return 66;
    }

    // Use the first device for subsequent operations
    const char *device = alldevs->name;
    bpf_u_int32 net = 0;
    bpf_u_int32 mask = 0;

    if (pcap_lookupnet(device, &net, &mask, errbuf) == -1) {
        // lookup failed; continue with net/mask = 0 (not fatal)
        net = 0;
        mask = 0;
    }

    // Open device for live capture (non-promiscuous)
    pcap_t *handle = pcap_open_live(device, 65535, 0, 1000, errbuf);
    if (handle == nullptr) {
        pcap_freealldevs(alldevs);
        return 2;
    }

    // Compile a simple filter ("ip")
    struct bpf_program fp;
    if (pcap_compile(handle, &fp, "ip", 1 /*optimize*/, net) == -1) {
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return 3;
    }

    // Attach the filter
    if (pcap_setfilter(handle, &fp) == -1) {
        pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return 4;
    }

    // Free compiled program (it's been applied)
    pcap_freecode(&fp);

    // Optionally fetch stats; not all capture types support this, ignore failure
    struct pcap_stat stats;
    if (pcap_stats(handle, &stats) == -1) {
        // ignore error, not fatal
    }

    // Close handle and free device list
    pcap_close(handle);
    pcap_freealldevs(alldevs);

    // If we reached here, the API sequence succeeded
    return 66;
}