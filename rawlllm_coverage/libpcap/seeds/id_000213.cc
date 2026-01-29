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
//<ID> 213
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpcap_api_sequence() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = nullptr;
    pcap_t *handle = nullptr;
    struct bpf_program fp;
    bpf_u_int32 netp = 0, maskp = 0;

    // 1) Find all devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // failure to find devices
        return -1;
    }

    if (alldevs == nullptr) {
        // no devices found
        pcap_freealldevs(alldevs);
        return -1;
    }

    // 2) Open the first device for live capture
    if (alldevs->name == nullptr) {
        pcap_freealldevs(alldevs);
        return -1;
    }

    handle = pcap_open_live(alldevs->name, BUFSIZ, 1, 1000, errbuf);
    if (handle == nullptr) {
        pcap_freealldevs(alldevs);
        return -1;
    }

    // 3) Try to look up network and mask for compiling filters (best-effort)
    if (pcap_lookupnet(alldevs->name, &netp, &maskp, errbuf) == -1) {
        // If lookup fails, continue with mask 0
        maskp = 0;
    }

    // 4) Compile and set a simple filter ("tcp" here)
    const char *filter_exp = "tcp";
    if (pcap_compile(handle, &fp, filter_exp, 1, maskp) == -1) {
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -1;
    }

    if (pcap_setfilter(handle, &fp) == -1) {
        pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -1;
    }
    // free compiled program after setting filter
    pcap_freecode(&fp);

    // 5) Read a single packet (non-blocking depending on timeout supplied to pcap_open_live)
    struct pcap_pkthdr *header = nullptr;
    const u_char *packet_data = nullptr;
    int res = pcap_next_ex(handle, &header, &packet_data);
    // res == 1 => got packet; 0 => timeout expired; -1 => error; -2 => EOF
    // We treat any outcome as acceptable for this API-sequence test; no further action required.

    // 6) Optionally get stats (best-effort)
    struct pcap_stat stats;
    (void)pcap_stats(handle, &stats); // ignore errors

    // 7) Query datalink type (best-effort)
    int dlt = pcap_datalink(handle);
    (void)dlt; // avoid unused-variable warning

    // 8) Clean up
    pcap_close(handle);
    pcap_freealldevs(alldevs);

    // Indicate success as requested
    return 66;
}