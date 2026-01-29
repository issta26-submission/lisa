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
//<ID> 206
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpcap_api_sequence() {
    // Note: this function assumes <pcap.h> is included by the caller/translation unit.
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = nullptr;
    pcap_t *handle = nullptr;
    struct bpf_program fp;
    bool compiled = false;

    // 1) Find all devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // failure finding devices
        return -1;
    }

    if (alldevs == nullptr) {
        // no devices found
        pcap_freealldevs(alldevs);
        return -2;
    }

    // Choose the first device
    const char *devname = alldevs->name;
    if (devname == nullptr) {
        pcap_freealldevs(alldevs);
        return -3;
    }

    // 2) Open the device for capture
    handle = pcap_open_live(devname, 65535, 1 /*promiscuous*/, 1000 /*ms timeout*/, errbuf);
    if (handle == nullptr) {
        pcap_freealldevs(alldevs);
        return -4;
    }

    // 3) Compile a simple filter ("tcp")
    if (pcap_compile(handle, &fp, "tcp", 1 /*optimize*/, PCAP_NETMASK_UNKNOWN) == -1) {
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -5;
    }
    compiled = true;

    // 4) Apply the compiled filter
    if (pcap_setfilter(handle, &fp) == -1) {
        if (compiled) pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return -6;
    }

    // 5) Set non-blocking mode (best-effort)
    if (pcap_setnonblock(handle, 1, errbuf) == -1) {
        // non-fatal for sequence demonstration; continue
    }

    // 6) Attempt a single non-blocking capture (pcap_next_ex)
    struct pcap_pkthdr *header = nullptr;
    const u_char *pkt_data = nullptr;
    int nex = pcap_next_ex(handle, &header, &pkt_data);
    // nex:
    //  1 -> packet read, 0 -> timeout in non-blocking mode (or timeout), -1 -> error, -2 -> EOF
    if (nex == -1) {
        // read error (non-fatal for our sequence, but record)
    }

    // 7) Prepare a small dummy packet and attempt to inject/send it.
    // Keep a small packet buffer; for Ethernet this will be mostly zero bytes.
    // Note: sending may fail (permissions, invalid packet, etc.) — handle gracefully.
    const unsigned int pkt_len = 60;
    unsigned char pkt[pkt_len];
    for (unsigned int i = 0; i < pkt_len; ++i) pkt[i] = 0x00;
    // Try to inject (pcap_inject returns number of bytes written or -1 on failure)
    int inj_res = pcap_inject(handle, pkt, pkt_len);
    (void)inj_res; // ignore result for demonstration

    // 8) Demonstrate pcap_stats (best-effort)
    struct pcap_stat stats;
    if (pcap_stats(handle, &stats) == 0) {
        // stats successfully retrieved (we don't use them further here)
    }

    // 9) Demonstrate breaking a loop (safe to call even if not looping)
    pcap_breakloop(handle);

    // Cleanup
    if (compiled) pcap_freecode(&fp);
    pcap_close(handle);
    pcap_freealldevs(alldevs);

    // If we reached here, the sequence of API calls completed successfully.
    return 66;
}