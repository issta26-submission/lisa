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
//<ID> 859
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initialization
    const char payload[] = "zlib payload for crc and gzprintf";
    const z_size_t payload_len = (z_size_t)(sizeof(payload) - 1);
    const Bytef *pbuf = (const Bytef *)payload;
    uLong crc_a = crc32_z(0UL, pbuf, payload_len);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");

    // step 2: Configure stream
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    strm.next_in = (Bytef *)pbuf;
    strm.avail_in = (uInt)payload_len;
    int rc_inflate_init = inflateInit_(&strm, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate - write and combine CRCs
    int rc_gz_printf = gzprintf(gf, "%s", payload);
    gzclearerr(gf);
    const char tail[] = "-tail";
    const z_size_t tail_len = (z_size_t)(sizeof(tail) - 1);
    uLong crc_b = crc32_z(0UL, (const Bytef *)tail, tail_len);
    uLong combined_crc = crc32_combine(crc_a, crc_b, (off_t)tail_len);

    // step 4: Validate and Cleanup
    strm.adler = combined_crc;
    int rc_inflate_end = inflateEnd(&strm);
    int rc_gz_close = gzclose(gf);
    (void)rc_inflate_init;
    (void)rc_gz_printf;
    (void)crc_a;
    (void)crc_b;
    (void)combined_crc;
    (void)rc_inflate_end;
    (void)rc_gz_close;
    // API sequence test completed successfully
    return 66;
}