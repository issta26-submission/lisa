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
//<ID> 215
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpcap_api_sequence() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = nullptr;

    // Find all devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        std::cerr << "pcap_findalldevs failed: " << errbuf << "\n";
        return 1;
    }

    if (!alldevs) {
        std::cerr << "No interfaces found\n";
        return 2;
    }

    // Pick the first device
    pcap_if_t *dev = alldevs;
    const char *devname = dev->name;
    if (!devname) {
        std::cerr << "Device has no name\n";
        pcap_freealldevs(alldevs);
        return 3;
    }

    // Lookup network and mask (best-effort)
    bpf_u_int32 netp = 0, maskp = 0;
    if (pcap_lookupnet(devname, &netp, &maskp, errbuf) == -1) {
        // not fatal; continue with zeroed net/mask
        netp = 0;
        maskp = 0;
    }

    // Open the device for live capture
    int snaplen = 65535;     // capture full packet
    int promisc = 0;         // non-promiscuous for safety
    int timeout_ms = 1000;   // 1 second
    pcap_t *handle = pcap_open_live(devname, snaplen, promisc, timeout_ms, errbuf);
    if (!handle) {
        std::cerr << "pcap_open_live failed: " << errbuf << "\n";
        pcap_freealldevs(alldevs);
        return 4;
    }

    // Compile a simple BPF filter
    struct bpf_program fp;
    const char *filter_exp = "tcp";
    if (pcap_compile(handle, &fp, filter_exp, 1 /* optimize */, netp) == -1) {
        std::cerr << "pcap_compile failed: " << pcap_geterr(handle) << "\n";
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return 5;
    }

    // Apply the filter
    if (pcap_setfilter(handle, &fp) == -1) {
        std::cerr << "pcap_setfilter failed: " << pcap_geterr(handle) << "\n";
        pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return 6;
    }

    // Try to capture a single packet (non-blocking up to timeout_ms)
    struct pcap_pkthdr *header = nullptr;
    const u_char *packet_data = nullptr;
    int rc = pcap_next_ex(handle, &header, &packet_data);
    if (rc == 1) {
        // Packet captured
        std::cout << "Captured packet: len=" << header->len
                  << ", caplen=" << header->caplen << "\n";
        // Example access to first few bytes (safe-check)
        size_t show = header->caplen < 16 ? header->caplen : 16;
        std::cout << "First " << show << " bytes:";
        for (size_t i = 0; i < show; ++i) {
            unsigned int byte = packet_data[i];
            std::cout << " " << std::hex << (byte >> 4) << (byte & 0xf) << std::dec;
        }
        std::cout << "\n";
    } else if (rc == 0) {
        std::cerr << "pcap_next_ex: timeout expired (no packet)\n";
        // Not fatal for demonstration
    } else if (rc == -1) {
        std::cerr << "pcap_next_ex error: " << pcap_geterr(handle) << "\n";
        pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return 7;
    } else if (rc == -2) {
        std::cerr << "pcap_next_ex: no more packets (offline capture)\n";
    }

    // Demonstrate retrieving information from the handle
    int dlt = pcap_datalink(handle);
    std::cout << "Data link type: " << dlt << "\n";

    // Clean up
    pcap_freecode(&fp);
    pcap_close(handle);
    pcap_freealldevs(alldevs);

    // Indicate overall success per specification
    return 66;
}