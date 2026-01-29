#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 993
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and Initialize
    const char payload[] = "zlib payload exercising gzprintf, gzwrite, inflatePrime and get_crc_table";
    const unsigned int payload_len = (unsigned int)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    const z_crc_t *crc_table = get_crc_table();
    z_crc_t crc_first = crc_table[0];
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    int rc_inflate_prime = inflatePrime(&istrm, 3, 5);

    // step 2: Configure (open gzip for writing and write header + payload)
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzprintf = gzprintf(gw, "CRC_TABLE_FIRST=%u\n", (unsigned int)crc_first);
    int rc_gzwrite = gzwrite(gw, (voidpc)src_buf, payload_len);
    int rc_gzflush = gzflush(gw, 0);
    int rc_gzclose_w = gzclose(gw);

    // step 3: Operate (reopen for reading and read back contents)
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    char outbuf[512];
    memset(outbuf, 0, sizeof(outbuf));
    int rc_gzread = gzread(gr, outbuf, (unsigned int)sizeof(outbuf));
    int rc_gzclose_r = gzclose(gr);

    // step 4: Validate and Cleanup
    int rc_inflate_end = inflateEnd(&istrm);
    (void)src_buf;
    (void)crc_table;
    (void)crc_first;
    (void)rc_inflate_init;
    (void)rc_inflate_prime;
    (void)rc_gzprintf;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzclose_w;
    (void)rc_gzread;
    (void)rc_gzclose_r;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}