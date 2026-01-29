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
//<ID> 480
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_libpcap_api_sequence() {
    // step 1: Initialize and create handle
    char errbuf[256];
    memset(errbuf, 0, sizeof(errbuf));
    pcap_init(0, errbuf);
    const char device[] = "any";
    pcap_t *handle = pcap_create(device, errbuf);
    pcap_set_immediate_mode(handle, 1);

    // step 2: Configure capture parameters
    pcap_set_snaplen(handle, 65535);
    pcap_set_promisc(handle, 1);
    pcap_set_timeout(handle, 1000);
    pcap_set_buffer_size(handle, 2 * 1024 * 1024);

    // step 3: Operate - activate, inspect datalinks, produce BPF image, and write a packet to a dumper
    pcap_activate(handle);
    int *dlt_list = NULL;
    int dlt_count = pcap_list_datalinks(handle, &dlt_list);
    int current_dlt = pcap_datalink(handle);
    struct bpf_insn insns[1];
    memset(insns, 0, sizeof(insns));
    insns[0].code = 0;
    insns[0].jt = 0;
    insns[0].jf = 0;
    insns[0].k = 0;
    char *insn_image = bpf_image(insns, 1);
    pcap_dumper_t *dumper = pcap_dump_open(handle, "test_dump.pcap");
    struct pcap_pkthdr hdr;
    memset(&hdr, 0, sizeof(hdr));
    hdr.caplen = 64;
    hdr.len = 64;
    hdr.ts.tv_sec = 1;
    hdr.ts.tv_usec = 0;
    unsigned char packet[64];
    memset(packet, 0xAB, sizeof(packet));
    pcap_dump((u_char *)dumper, &hdr, packet);

    // step 4: Validate and cleanup
    int flush_ret = pcap_dump_flush(dumper);
    pcap_dump_close(dumper);
    pcap_free_datalinks(dlt_list);
    (void)insn_image; // insn_image must not be freed here (not allocated by malloc)
    pcap_close(handle);

    puts("API sequence test completed successfully");
    (void)device;
    (void)errbuf;
    (void)dlt_count;
    (void)current_dlt;
    (void)flush_ret;
    return 66;
}