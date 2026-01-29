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
//<ID> 187
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
    bpf_program fp; // compiled filter
    bool compiled = false;
    bool opened = false;
    bool found_packet = false;

    // 1) Find all devices
    if (pcap_findalldevs(&alldevs, errbuf) != 0) {
        std::cerr << "pcap_findalldevs failed: " << errbuf << "\n";
        return -1;
    }
    if (alldevs == nullptr) {
        std::cerr << "No devices found by pcap_findalldevs\n";
        return -2;
    }

    // Choose the first available device
    pcap_if_t* dev = alldevs;

    // 2) Open the device for live capture
    std::memset(errbuf, 0, sizeof(errbuf));
    // snapshot length BUFSIZ, promiscuous mode = 1, timeout = 1000 ms
    handle = pcap_open_live(dev->name, BUFSIZ, 1, 1000, errbuf);
    if (handle == nullptr) {
        std::cerr << "pcap_open_live failed: " << errbuf << "\n";
        pcap_freealldevs(alldevs);
        return -3;
    }
    opened = true;

    // 3) Lookup network and mask for the device (used by pcap_compile)
    bpf_u_int32 net = 0;
    bpf_u_int32 mask = 0;
    if (pcap_lookupnet(dev->name, &net, &mask, errbuf) != 0) {
        // Not fatal: we can continue with a default mask (0)
        std::cerr << "pcap_lookupnet failed for device " << dev->name << ": " << errbuf << "\n";
        mask = 0;
    }

    // 4) Compile a BPF filter (example: capture only TCP packets)
    const char* filter_exp = "tcp";
    if (pcap_compile(handle, &fp, filter_exp, 1 /*optimize*/, mask) != 0) {
        std::cerr << "pcap_compile failed: " << pcap_geterr(handle) << "\n";
        if (opened) pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -4;
    }
    compiled = true;

    // 5) Set the compiled filter
    if (pcap_setfilter(handle, &fp) != 0) {
        std::cerr << "pcap_setfilter failed: " << pcap_geterr(handle) << "\n";
        pcap_freecode(&fp);
        if (opened) pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -5;
    }

    // 6) Attempt to capture a single packet (with limited tries to avoid blocking forever)
    int max_attempts = 5;
    int attempts = 0;
    while (attempts < max_attempts) {
        struct pcap_pkthdr* header = nullptr;
        const u_char* packet = nullptr;
        int res = pcap_next_ex(handle, &header, &packet);
        if (res == 1) {
            // packet captured
            if (header != nullptr && packet != nullptr && header->caplen > 0) {
                found_packet = true;
                break;
            }
        } else if (res == 0) {
            // timeout expired, try again
            attempts++;
            continue;
        } else if (res == -1) {
            // error
            std::cerr << "pcap_next_ex error: " << pcap_geterr(handle) << "\n";
            break;
        } else if (res == -2) {
            // EOF (offline capture ended), break
            break;
        }
        attempts++;
    }

    // Cleanup
    if (compiled) pcap_freecode(&fp);
    if (opened) pcap_close(handle);
    pcap_freealldevs(alldevs);

    if (!found_packet) {
        std::cerr << "No packet captured that matched the filter within attempts\n";
        return -6;
    }

    // If we reach here, the sequence of libpcap API calls succeeded
    return 66;
}