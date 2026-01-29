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
//<ID> 74
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpcap_api_sequence() {
    char errbuf[PCAP_ERRBUF_SIZE];

    // 1) Find all devices
    pcap_if_t *alldevs = nullptr;
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // failure to enumerate devices
        return -1;
    }

    if (alldevs == nullptr) {
        // no devices found
        return -2;
    }

    // 2) Open the first available device for live capture
    const char *dev = alldevs->name;
    pcap_t *handle = pcap_open_live(dev, BUFSIZ, 1 /* promisc */, 1000 /* ms timeout */, errbuf);
    if (handle == nullptr) {
        pcap_freealldevs(alldevs);
        return -3;
    }

    // 3) Try to make the handle non-blocking (non-fatal if it fails; treat failure as non-fatal here)
    if (pcap_setnonblock(handle, 1, errbuf) == -1) {
        // not fatal for this test sequence; continue
    }

    // 4) Compile a BPF filter and set it on the handle
    struct bpf_program filter;
    const char *filter_exp = "tcp port 80";
    if (pcap_compile(handle, &filter, filter_exp, 1 /* optimize */, PCAP_NETMASK_UNKNOWN) == -1) {
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -4;
    }

    if (pcap_setfilter(handle, &filter) == -1) {
        pcap_freecode(&filter);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -5;
    }

    // 5) Attempt to retrieve a single packet (non-blocking or timeout may cause 0)
    struct pcap_pkthdr *header = nullptr;
    const u_char *packet = nullptr;
    int res = pcap_next_ex(handle, &header, &packet);
    if (res == 1) {
        // packet received; perform a minimal safe access to packet data
        if (header && packet && header->caplen > 0) {
            volatile unsigned char first_byte = packet[0];
            (void)first_byte; // avoid unused-variable warning
        }
    } else if (res == 0) {
        // timeout expired (no packet) - acceptable for this sequence
    } else if (res == -1) {
        // error while reading
        pcap_freecode(&filter);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -6;
    } else if (res == -2) {
        // offline read complete (not expected for live capture) - continue
    }

    // 6) Optionally fetch statistics (may fail on some capture types; treat failure as non-fatal)
    struct pcap_stat stats;
    if (pcap_stats(handle, &stats) == 0) {
        // use stats minimally to demonstrate access
        (void)stats.ps_recv;
        (void)stats.ps_drop;
#ifdef HAVE_PCAP_STAT_TPS
        (void)stats.ps_ifdrop;
#endif
    }

    // 7) Cleanup
    pcap_freecode(&filter);
    pcap_close(handle);
    pcap_freealldevs(alldevs);

    // Success as requested
    return 66;
}