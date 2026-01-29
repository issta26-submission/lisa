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
//<ID> 96
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpcap_api_sequence() {
    // Include here so the function is self-contained for the snippet.
    // (Including inside a function is allowed by the preprocessor.)
    #include <pcap/pcap.h>

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t* alldevs = nullptr;

    // Discover devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // failure to find devices
        return -1;
    }
    if (alldevs == nullptr) {
        // no devices found
        return -2;
    }

    // Pick the first device
    const char* dev = alldevs->name;
    if (dev == nullptr) {
        pcap_freealldevs(alldevs);
        return -3;
    }

    // Open the device for live capture (non-promiscuous, 1s timeout)
    pcap_t* handle = pcap_open_live(dev, 65536, 0, 1000, errbuf);
    if (handle == nullptr) {
        pcap_freealldevs(alldevs);
        return -4;
    }

    // Compile and set a simple filter (capture only IP packets)
    struct bpf_program fp;
    const char* filter_exp = "ip";
    if (pcap_compile(handle, &fp, filter_exp, 1, PCAP_NETMASK_UNKNOWN) == -1) {
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -5;
    }
    if (pcap_setfilter(handle, &fp) == -1) {
        pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -6;
    }

    // Attempt to fetch a single packet (this may timeout)
    struct pcap_pkthdr* header = nullptr;
    const u_char* packet = nullptr;
    int r = pcap_next_ex(handle, &header, &packet);
    if (r == 1) {
        // Successfully got a packet; do minimal safe inspection
        if (header && header->caplen > 0 && packet) {
            volatile unsigned char first_byte = packet[0];
            (void)first_byte; // avoid unused-variable warnings
        }
    } else if (r == 0) {
        // timeout; that's acceptable for this sequence (APIs worked)
    } else {
        // r == -1 (error) or r == -2 (EOF / no more packets on savefile)
        // Treat as non-fatal here because earlier API calls succeeded,
        // but we still perform cleanup and return a non-success code.
        pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -7;
    }

    // Clean up
    pcap_freecode(&fp);
    pcap_close(handle);
    pcap_freealldevs(alldevs);

    // Indicate overall success as requested
    return 66;
}