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
//<ID> 33
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

#if __has_include(<pcap/pcap.h>)
#elif __has_include(<pcap.h>)
#else
#error "pcap header not found"
#endif

int test_libpcap_api_sequence() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = nullptr;
    pcap_t *handle = nullptr;
    bpf_program fp;
    bool compiled = false;

    // 1) Find all devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // failure to find devices
        return -1;
    }

    if (alldevs == nullptr) {
        // no devices found
        return -2;
    }

    // 2) Choose the first device
    const char *device_name = alldevs->name;
    if (device_name == nullptr || std::strlen(device_name) == 0) {
        pcap_freealldevs(alldevs);
        return -3;
    }

    // 3) Open the device for capturing (non-promiscuous, 1s timeout)
    handle = pcap_open_live(device_name, 65535, 0 /*promisc*/, 1000 /*ms*/, errbuf);
    if (handle == nullptr) {
        pcap_freealldevs(alldevs);
        return -4;
    }

    // 4) Check datalink type (example usage)
    int dlt = pcap_datalink(handle);
    (void)dlt; // use dlt if needed; avoid unused-variable warning

    // 5) Compile a simple filter ("tcp")
    const char *filter_exp = "tcp";
    bpf_u_int32 netmask = 0; // if unknown, 0 is acceptable for compilation
    if (pcap_compile(handle, &fp, filter_exp, 1 /*optimize*/, netmask) == -1) {
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -5;
    }
    compiled = true;

    // 6) Apply the compiled filter
    if (pcap_setfilter(handle, &fp) == -1) {
        pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -6;
    }

    // 7) Attempt to read a single packet (non-fatal if timeout or none)
    struct pcap_pkthdr *header = nullptr;
    const u_char *pkt_data = nullptr;
    int res = pcap_next_ex(handle, &header, &pkt_data);
    // res == 1 -> packet read, 0 -> timeout, -1 -> error, -2 -> EOF on savefile
    // We don't treat timeout/no-packet as failure for this API-sequence test.
    (void)res;
    (void)header;
    (void)pkt_data;

    // 8) Clean up compiled program and handles
    if (compiled) {
        pcap_freecode(&fp);
        compiled = false;
    }
    pcap_close(handle);
    pcap_freealldevs(alldevs);

    // Success
    return 66;
}