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
//<ID> 855
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib_api_sequence_payload_for_crc_and_gzprintf";
    const uLong payloadLen = (uLong)(sizeof(payload) - 1);
    const Bytef *source = (const Bytef *)payload;
    uLong crc1 = crc32_z(0UL, source, (z_size_t)payloadLen);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)payloadLen;

    // step 2: Initialize inflate stream (prepare for potential inflate operations)
    int rc_inflate_init = inflateInit_(&strm, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate and validate with CRCs and gzprintf
    uLong crc2 = crc32_z(crc1, source, (z_size_t)payloadLen);
    uLong combined_crc = crc32_combine(crc1, crc2, (off_t)payloadLen);
    int rc_printf = gzprintf(gf, "%lu\n", (unsigned long)combined_crc);
    gzclearerr(gf);

    // step 4: Cleanup
    int rc_close = gzclose(gf);
    int rc_inflate_end = inflateEnd(&strm);

    (void)rc_inflate_init;
    (void)rc_printf;
    (void)rc_close;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}