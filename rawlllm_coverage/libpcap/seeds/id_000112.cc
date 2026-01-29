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
//<ID> 112
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpcap_api_sequence() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t* alldevs = nullptr;

    // Find all devices
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        // pcap_findalldevs reports error in errbuf
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

    // Open the device for live capture
    pcap_t* handle = pcap_open_live(dev, BUFSIZ, 1 /* promiscuous */, 1000 /* ms timeout */, errbuf);
    if (handle == nullptr) {
        pcap_freealldevs(alldevs);
        return 4;
    }

    // Query link-layer header type (demonstrates another API)
    int dlt = pcap_datalink(handle);
    (void)dlt; // suppress unused variable warning; in real code you'd use it

    // Compile a simple filter
    struct bpf_program fp;
    const char* filter_exp = "tcp port 80";
    if (pcap_compile(handle, &fp, filter_exp, 1 /* optimize */, PCAP_NETMASK_UNKNOWN) == -1) {
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return 5;
    }

    // Set the compiled filter
    if (pcap_setfilter(handle, &fp) == -1) {
        pcap_freecode(&fp);
        pcap_close(handle);
        pcap_freealldevs(alldevs);
        return 6;
    }

    // Try to read up to one packet (use pcap_next_ex to avoid callbacks)
    struct pcap_pkthdr* header = nullptr;
    const u_char* packet = nullptr;
    int got_packet = 0;
    for (int attempt = 0; attempt < 3; ++attempt) {
        int res = pcap_next_ex(handle, &header, &packet);
        if (res == 1) { // packet read
            got_packet = 1;
            // minimally touch packet data to show usage
            if (header->caplen > 0 && packet != nullptr) {
                volatile unsigned char first_byte = packet[0];
                (void)first_byte;
            }
            break;
        } else if (res == 0) {
            // timeout expired, retry
            continue;
        } else if (res == -1) {
            // error during read
            pcap_freecode(&fp);
            pcap_close(handle);
            pcap_freealldevs(alldevs);
            return 7;
        } else if (res == -2) {
            // no more packets when reading from savefile; break
            break;
        }
    }

    // Retrieve capture statistics (may fail on some platforms)
    struct pcap_stat stats;
    if (pcap_stats(handle, &stats) == 0) {
        // Use stats minimally to show API usage
        (void)stats.ps_recv;
    }

    // Cleanup
    pcap_freecode(&fp);
    pcap_close(handle);
    pcap_freealldevs(alldevs);

    // If we reached here, the API sequence completed successfully
    return 66;
}