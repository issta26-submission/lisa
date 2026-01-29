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
//<ID> 104
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
    struct bpf_program fp{};
    bool fp_compiled = false;

    // 1) Find all devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        std::fprintf(stderr, "pcap_findalldevs failed: %s\n", errbuf);
        return -1;
    }

    if (alldevs == nullptr) {
        // No devices found
        std::fprintf(stderr, "pcap_findalldevs succeeded but returned no devices\n");
        // nothing to free in this case
        return -2;
    }

    // 2) Pick the first device
    const char *dev = alldevs->name;
    if (dev == nullptr) {
        std::fprintf(stderr, "No device name available\n");
        pcap_freealldevs(alldevs);
        return -3;
    }

    // 3) Lookup network and netmask (not fatal if it fails; use 0)
    bpf_u_int32 netp = 0, maskp = 0;
    if (pcap_lookupnet(dev, &netp, &maskp, errbuf) == -1) {
        // Not fatal: proceed with netmask 0
        std::fprintf(stderr, "pcap_lookupnet failed for %s: %s (continuing with mask 0)\n", dev, errbuf);
        netp = 0;
        maskp = 0;
    }

    // 4) Open the device for live capture
    // snaplen: 65535, promisc: 1, timeout: 1000 ms
    handle = pcap_open_live(dev, 65535, 1, 1000, errbuf);
    if (handle == nullptr) {
        std::fprintf(stderr, "pcap_open_live failed for %s: %s\n", dev, errbuf);
        pcap_freealldevs(alldevs);
        return -4;
    }

    // 5) Compile a simple filter (capture IP packets)
    const char *filter_exp = "ip";
    if (pcap_compile(handle, &fp, filter_exp, 1, maskp) == -1) {
        std::fprintf(stderr, "pcap_compile failed: %s\n", pcap_geterr(handle));
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -5;
    }
    fp_compiled = true;

    // 6) Set the compiled filter
    if (pcap_setfilter(handle, &fp) == -1) {
        std::fprintf(stderr, "pcap_setfilter failed: %s\n", pcap_geterr(handle));
        pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -6;
    }

    // 7) Attempt to read a single packet (non-blocking behavior depends on device & timeout)
    const u_char *packet_data = nullptr;
    struct pcap_pkthdr *header = nullptr;
    int rc = pcap_next_ex(handle, &header, &packet_data);
    if (rc == 1) {
        // Packet captured; header and packet_data valid
        // (We won't process contents here; just demonstrate access)
        std::fprintf(stdout, "Captured one packet: caplen=%u, len=%u\n",
                     header->caplen, header->len);
    } else if (rc == 0) {
        // Timeout elapsed
        std::fprintf(stdout, "pcap_next_ex timeout (no packet available within timeout)\n");
    } else if (rc == -1) {
        // Error occurred
        std::fprintf(stderr, "pcap_next_ex error: %s\n", pcap_geterr(handle));
        // Continue to cleanup but treat as non-fatal for this demonstration
    } else if (rc == -2) {
        // EOF was reached in offline capture (shouldn't happen for live)
        std::fprintf(stdout, "pcap_next_ex returned EOF\n");
    }

    // 8) Retrieve statistics (may fail on some platforms; treat failure as non-fatal)
    struct pcap_stat stats{};
    if (pcap_stats(handle, &stats) == 0) {
        std::fprintf(stdout, "pcap_stats: received=%u, dropped=%u, if_dropped=%u\n",
                     stats.ps_recv, stats.ps_drop, stats.ps_ifdrop);
    } else {
        std::fprintf(stderr, "pcap_stats not supported or failed: %s\n", pcap_geterr(handle));
    }

    // 9) Cleanup resources
    if (fp_compiled) {
        pcap_freecode(&fp);
    }
    pcap_close(handle);
    pcap_freealldevs(alldevs);

    // Success
    return 66;
}