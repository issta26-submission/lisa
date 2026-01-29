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
//<ID> 86
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpcap_api_sequence() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = nullptr;
    pcap_if_t *dev = nullptr;
    pcap_t *handle = nullptr;
    struct bpf_program fp;
    bool compiled = false;

    // 1) Find all devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // If findalldevs fails, try the deprecated lookupdev as a last resort
        const char *fallback = pcap_lookupdev(errbuf);
        if (!fallback) {
            std::fprintf(stderr, "pcap_findalldevs failed: %s\n", errbuf);
            return -1;
        }
        // build a fake single-device list (we will simply open fallback device)
        // Note: we won't use pcap_freealldevs in this fallback path
        handle = pcap_open_live(fallback, 65535, 1, 1000, errbuf);
        if (!handle) {
            std::fprintf(stderr, "pcap_open_live (fallback) failed: %s\n", errbuf);
            return -2;
        }
    } else {
        // 2) Select the first device returned
        dev = alldevs;
        if (!dev) {
            std::fprintf(stderr, "No devices found by pcap_findalldevs\n");
            pcap_freealldevs(alldevs);
            return -3;
        }

        // 3) Open the device for live capture
        handle = pcap_open_live(dev->name, 65535, 1 /*promisc*/, 1000 /*ms timeout*/, errbuf);
        if (!handle) {
            std::fprintf(stderr, "pcap_open_live failed on device %s: %s\n",
                         dev->name, errbuf);
            pcap_freealldevs(alldevs);
            return -4;
        }
    }

    // From here on we have a valid handle (either from fallback or opened device list)
    // 4) Get some info (snapshot length and datalink)
    int snaplen = pcap_snapshot(handle);
    int datalink = pcap_datalink(handle);
    std::printf("Opened handle: snaplen=%d, datalink=%d\n", snaplen, datalink);

    // 5) Compile a simple filter ("tcp")
    const char *filter_exp = "tcp";
    if (pcap_compile(handle, &fp, filter_exp, 1 /*optimize*/, PCAP_NETMASK_UNKNOWN) == -1) {
        std::fprintf(stderr, "pcap_compile failed: %s\n", pcap_geterr(handle));
        pcap_close(handle);
        if (alldevs) pcap_freealldevs(alldevs);
        return -5;
    }
    compiled = true;

    // 6) Set the compiled filter
    if (pcap_setfilter(handle, &fp) == -1) {
        std::fprintf(stderr, "pcap_setfilter failed: %s\n", pcap_geterr(handle));
        if (compiled) pcap_freecode(&fp);
        pcap_close(handle);
        if (alldevs) pcap_freealldevs(alldevs);
        return -6;
    }

    // 7) Optionally set non-blocking mode (demonstrate API)
    if (pcap_setnonblock(handle, 1, errbuf) == -1) {
        // Not a fatal error for this test; just print a warning.
        std::fprintf(stderr, "pcap_setnonblock warning: %s\n", errbuf);
    } else {
        std::printf("Set pcap non-blocking mode\n");
    }

    // 8) Try to capture up to a few packets with pcap_next_ex (non-fatal if none)
    int packets_to_capture = 5;
    int captured = 0;
    for (int i = 0; i < packets_to_capture; ++i) {
        struct pcap_pkthdr *header = nullptr;
        const u_char *data = nullptr;
        int res = pcap_next_ex(handle, &header, &data);
        if (res == 1) {
            // Got a packet
            std::printf("Packet %d: caplen=%u, len=%u\n", captured + 1,
                        header->caplen, header->len);
            ++captured;
        } else if (res == 0) {
            // Timeout elapsed in non-blocking mode or no packet available
            std::printf("Timeout/no-packet (iteration %d)\n", i + 1);
            // Sleep briefly to avoid a tight loop if desired; here we just continue.
        } else if (res == -1) {
            std::fprintf(stderr, "Error reading the packet: %s\n", pcap_geterr(handle));
            break;
        } else if (res == -2) {
            std::printf("No more packets (pcap_next_ex returned -2)\n");
            break;
        }
    }
    std::printf("Captured %d packet(s) in test loop\n", captured);

    // 9) Optionally read statistics (best-effort)
    struct pcap_stat stats;
    if (pcap_stats(handle, &stats) == 0) {
        std::printf("pcap stats: received=%u, dropped=%u, if_dropped=%u\n",
                    stats.ps_recv, stats.ps_drop, stats.ps_ifdrop);
    } else {
        // Some capture types don't support stats; not fatal
        std::printf("pcap_stats not supported: %s\n", pcap_geterr(handle));
    }

    // 10) Cleanup
    if (compiled) pcap_freecode(&fp);
    pcap_close(handle);
    if (alldevs) pcap_freealldevs(alldevs);

    // If we reached this point, sequence is considered successful
    return 66;
}