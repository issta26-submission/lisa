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
//<ID> 927
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and gzFile write + query compile flags
    const char payload[] = "zlib api sequence payload for crc32 combine and inflate cleanup";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    uLong compile_flags = zlibCompileFlags();
    unsigned int to_write = (unsigned int)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_gzwrite = gzwrite(gf, (voidpc)payload, to_write);
    int rc_gzflush = gzflush(gf, 0);

    // step 2: Compute CRCs for two contiguous segments and combine them
    uInt half_len = (uInt)(payload_len / 2);
    uInt second_len = (uInt)(payload_len - (uLong)half_len);
    uLong crc_first = crc32(0UL, (const Bytef *)payload, half_len);
    uLong crc_second = crc32(0UL, (const Bytef *)(payload + half_len), second_len);
    uLong crc_combined = crc32_combine(crc_first, crc_second, (off_t)second_len);

    // step 3: Initialize an inflate stream, configure with the payload as input, then cleanup
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));
    istrm.next_in = (Bytef *)payload;
    istrm.avail_in = (uInt)(payload_len <= (uLong)~0u ? payload_len : (uLong)~0u);
    int rc_inflate_end = inflateEnd(&istrm);

    // step 4: Final cleanup of gzFile and mark results used
    int rc_gzclose = gzclose(gf);
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)compile_flags;
    (void)crc_first;
    (void)crc_second;
    (void)crc_combined;
    (void)rc_inflate_init;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}