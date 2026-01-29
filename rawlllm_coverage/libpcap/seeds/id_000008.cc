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
//<ID> 8
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpcap_api_sequence() {
    /* include here so the function is self-contained when pasted into a file */
    #include <pcap/pcap.h>
    #include <cstring>

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_if_t *alldevs = nullptr;
    const char *device = nullptr;

    /* Find all devices */
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        /* fallback to lookupdev if findalldevs fails */
        const char *dev = pcap_lookupdev(errbuf);
        if (!dev) {
            /* cannot find any device */
            return -1;
        }
        device = dev;
    } else {
        if (alldevs == nullptr) {
            /* no devices found, try lookupdev */
            const char *dev = pcap_lookupdev(errbuf);
            if (!dev) {
                return -2;
            }
            device = dev;
        } else {
            /* use the first device in the list */
            device = alldevs->name;
        }
    }

    /* Open the device for live capture */
    pcap_t *handle = pcap_open_live(device,        /* device name */
                                    65535,         /* snaplen */
                                    0,             /* promisc (0 = non-promiscuous) */
                                    1000,          /* read timeout in ms */
                                    errbuf);       /* error buffer */
    if (handle == nullptr) {
        if (alldevs) pcap_freealldevs(alldevs);
        return -3;
    }

    /* Get network/netmask for the device (optional, used for compiling filters) */
    bpf_u_int32 net = 0, netmask = 0;
    if (pcap_lookupnet(device, &net, &netmask, errbuf) == -1) {
        /* If lookup fails, use 0 as netmask (or PCAP_NETMASK_UNKNOWN if available) */
#ifdef PCAP_NETMASK_UNKNOWN
        netmask = PCAP_NETMASK_UNKNOWN;
#else
        netmask = 0;
#endif
    }

    /* Compile and set a simple filter (example: capture only TCP packets) */
    struct bpf_program fp;
    const char *filter_exp = "tcp";
    if (pcap_compile(handle, &fp, filter_exp, 1, netmask) == -1) {
        pcap_close(handle);
        if (alldevs) pcap_freealldevs(alldevs);
        return -4;
    }
    if (pcap_setfilter(handle, &fp) == -1) {
        pcap_freecode(&fp);
        pcap_close(handle);
        if (alldevs) pcap_freealldevs(alldevs);
        return -5;
    }

    /* Inspect datalink type and snapshot length */
    int dlt = pcap_datalink(handle);
    int snaplen = pcap_snapshot(handle);
    (void)dlt;    /* use (void) to silence unused-variable warnings if necessary */
    (void)snaplen;

    /* Try fetching a single packet (will return 0 on timeout, 1 on packet, -1 on error) */
    struct pcap_pkthdr *header = nullptr;
    const u_char *packet = nullptr;
    int rv = pcap_next_ex(handle, &header, &packet);
    if (rv == -1) {
        /* error while reading a packet */
        pcap_freecode(&fp);
        pcap_close(handle);
        if (alldevs) pcap_freealldevs(alldevs);
        return -6;
    }
    /* rv == 0 means timeout, rv == 1 means a packet was returned, rv == -2 means EOF for offline */

    /* Retrieve statistics (may fail if not supported) */
    struct pcap_stat stats;
    if (pcap_stats(handle, &stats) == -1) {
        /* ignore stats failure, not fatal for this test */
    }

    /* Clean up */
    pcap_freecode(&fp);
    pcap_close(handle);
    if (alldevs) pcap_freealldevs(alldevs);

    /* If we reached here, the API sequence succeeded */
    return 66;
}