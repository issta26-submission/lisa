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
//<ID> 723
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_libpcap_api_sequence() {
    char errbuf[256];
    memset(errbuf, 0, sizeof(errbuf));
    char *dev = pcap_lookupdev(errbuf);
    pcap_t *live_handle = pcap_create(dev, errbuf);

    int snap_ret = pcap_set_snaplen(live_handle, 65535);
    int promisc_ret = pcap_set_promisc(live_handle, 1);
    int rfmon_ret = pcap_set_rfmon(live_handle, 1);
    int timeout_ret = pcap_set_timeout(live_handle, 1000);

    int act_ret = pcap_activate(live_handle);
    int selectable_fd = pcap_get_selectable_fd(live_handle);

    FILE *tmp = tmpfile();
    unsigned int magic = 0xa1b2c3d4u;
    unsigned short version_major = 2;
    unsigned short version_minor = 4;
    int thiszone = 0;
    unsigned int sigfigs = 0;
    unsigned int snaplen = 65535u;
    unsigned int network = 1u;
    fwrite(&magic, sizeof(magic), 1, tmp);
    fwrite(&version_major, sizeof(version_major), 1, tmp);
    fwrite(&version_minor, sizeof(version_minor), 1, tmp);
    fwrite(&thiszone, sizeof(thiszone), 1, tmp);
    fwrite(&sigfigs, sizeof(sigfigs), 1, tmp);
    fwrite(&snaplen, sizeof(snaplen), 1, tmp);
    fwrite(&network, sizeof(network), 1, tmp);
    unsigned int ts_sec = 0;
    unsigned int ts_usec = 0;
    unsigned int incl_len = 4;
    unsigned int orig_len = 4;
    unsigned char pkt[4] = { 0x00, 0x01, 0x02, 0x03 };
    fwrite(&ts_sec, sizeof(ts_sec), 1, tmp);
    fwrite(&ts_usec, sizeof(ts_usec), 1, tmp);
    fwrite(&incl_len, sizeof(incl_len), 1, tmp);
    fwrite(&orig_len, sizeof(orig_len), 1, tmp);
    fwrite(pkt, 1, 4, tmp);
    rewind(tmp);
    pcap_t *offline_handle = pcap_fopen_offline(tmp, errbuf);
    pcap_dumper_t *dumper = pcap_dump_open(offline_handle, "test_capture.pcap");
    struct pcap_pkthdr header;
    memset(&header, 0, sizeof(header));
    const u_char *packet = pcap_next(offline_handle, &header);
    pcap_dump((u_char *)dumper, &header, packet);
    int flush_ret = pcap_dump_flush(dumper);

    pcap_dump_close(dumper);
    pcap_close(offline_handle);
    /* Do not call fclose(tmp); pcap_close on an offline handle created with
       pcap_fopen_offline() closes the FILE* internally, so calling fclose(tmp)
       as well would cause a double free. */
    pcap_close(live_handle);
    puts("API sequence test completed successfully");

    (void)errbuf;
    (void)dev;
    (void)snap_ret;
    (void)promisc_ret;
    (void)rfmon_ret;
    (void)timeout_ret;
    (void)act_ret;
    (void)selectable_fd;
    (void)pkt;
    (void)packet;
    (void)header;
    (void)flush_ret;

    return 66;
}