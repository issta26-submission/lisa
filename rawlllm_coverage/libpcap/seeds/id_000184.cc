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
//<ID> 184
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpcap_api_sequence() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t* alldevs = nullptr;
    pcap_t* handle = nullptr;
    struct bpf_program fp;
    bool compiled = false;

    // Find available devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // error finding devices
        return -1;
    }
    if (!alldevs) {
        // no devices found
        return -1;
    }

    // Choose the first device
    const char* dev = alldevs->name;
    if (!dev) {
        pcap_freealldevs(alldevs);
        return -1;
    }

    // Open the device for live capture
    handle = pcap_open_live(dev, 65536, 0 /* non-promiscuous */, 1000 /* ms timeout */, errbuf);
    if (!handle) {
        pcap_freealldevs(alldevs);
        return -1;
    }

    // Compile a simple filter ("ip")
    const char* filter_exp = "ip";
    bpf_u_int32 netmask = 0xffffffff;
    if (pcap_compile(handle, &fp, filter_exp, 1 /* optimize */, netmask) == -1) {
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -1;
    }
    compiled = true;

    // Set the compiled filter
    if (pcap_setfilter(handle, &fp) == -1) {
        pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -1;
    }

    // Set the handle to non-blocking mode (demonstrates another API)
    if (pcap_setnonblock(handle, 1, errbuf) == -1) {
        // cleanup and fail
        pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -1;
    }

    // Query statistics (may succeed or fail depending on kernel/driver)
    struct pcap_stat stats;
    (void)pcap_stats(handle, &stats); // ignore return; just demonstrate API

    // Try to fetch a single packet (non-blocking due to previous call)
    struct pcap_pkthdr* header = nullptr;
    const u_char* pkt_data = nullptr;
    int rc = pcap_next_ex(handle, &header, &pkt_data);
    if (rc == -1) {
        // error while reading packet
        if (compiled) pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -1;
    }
    // rc == 1 => packet read, rc == 0 => timeout (no packet), rc == -2 => EOF
    // We treat timeout/EOF as non-fatal for this synthetic sequence.

    // Cleanup
    if (compiled) pcap_freecode(&fp);
    pcap_close(handle);
    pcap_freealldevs(alldevs);

    // Indicate overall success as requested
    return 66;
}