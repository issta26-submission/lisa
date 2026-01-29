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
//<ID> 179
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpcap_api_sequence() {
    char errbuf[PCAP_ERRBUF_SIZE];

    // 1) Find all devices
    pcap_if_t* alldevs = nullptr;
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // Failed to find devices
        return 1;
    }

    if (alldevs == nullptr) {
        // No devices found
        return 2;
    }

    // Choose the first device
    const char* dev = alldevs->name;
    if (dev == nullptr) {
        pcap_freealldevs(alldevs);
        return 3;
    }

    // 2) Open the device for live capture
    // snaplen: 65535, promiscuous: 1, timeout_ms: 1000
    pcap_t* handle = pcap_open_live(dev, 65535, 1, 1000, errbuf);
    if (handle == nullptr) {
        pcap_freealldevs(alldevs);
        return 4;
    }

    // 3) Compile a simple filter (capture IP packets)
    struct bpf_program fp;
    const char* filter_exp = "ip";
    bpf_u_int32 netmask = 0; // unknown netmask; OK for simple filters like "ip"
    if (pcap_compile(handle, &fp, filter_exp, 1, netmask) == -1) {
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return 5;
    }

    // 4) Set the compiled filter
    if (pcap_setfilter(handle, &fp) == -1) {
        pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return 6;
    }

    // 5) Capture a single packet (with timeout handled by pcap_next_ex)
    struct pcap_pkthdr* header = nullptr;
    const u_char* packet = nullptr;
    int res = pcap_next_ex(handle, &header, &packet);
    // res == 1 : packet read
    // res == 0 : timeout
    // res == -1: error
    // res == -2: EOF (offline capture)
    if (res != 1) {
        // Clean up
        pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return 7;
    }

    // Optionally examine header->caplen, header->len or packet contents here.
    // For this test sequence we just ensure we successfully retrieved a packet.

    // 6) Cleanup
    pcap_freecode(&fp);
    pcap_close(handle);
    pcap_freealldevs(alldevs);

    // Success: return 66 as requested
    return 66;
}