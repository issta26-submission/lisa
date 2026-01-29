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
//<ID> 1230
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payloadA[] = "Hello_zlib_part_A";
    const char payloadB[] = "World_zlib_part_B";
    const uLong lenA = (uLong)(sizeof(payloadA) - 1);
    const uLong lenB = (uLong)(sizeof(payloadB) - 1);
    const uLong total_len = (uLong)(lenA + lenB);
    const char *version = zlibVersion();
    uLong crcA = crc32(0UL, (const Bytef *)payloadA, (uInt)lenA);
    uLong crcB = crc32(0UL, (const Bytef *)payloadB, (uInt)lenB);
    uLong crc_combined = crc32_combine(crcA, crcB, (off_t)lenB);
    Bytef *src = new Bytef[total_len];
    memset(src, 0, (size_t)total_len);
    memcpy(src, payloadA, (size_t)lenA);
    memcpy(src + lenA, payloadB, (size_t)lenB);
    uLongf comp_capacity = compressBound(total_len);
    Bytef *comp_buf = new Bytef[(size_t)comp_capacity];
    memset(comp_buf, 0, (size_t)comp_capacity);
    uLongf comp_len = comp_capacity;
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (compress source and initialize inflate stream)
    int rc_compress = compress2(comp_buf, &comp_len, src, (uLong)total_len, 6);
    int rc_inflate_init = inflateInit_(&strm, version, (int)sizeof(z_stream));

    // step 3: Core operations (uncompress, validate inflate state, write to gz)
    uLong source_len_for_uncompress = (uLong)comp_len;
    uLongf decomp_capacity = total_len + 16;
    Bytef *decomp_buf = new Bytef[(size_t)decomp_capacity];
    memset(decomp_buf, 0, (size_t)decomp_capacity);
    int rc_uncompress = uncompress2(decomp_buf, &decomp_capacity, comp_buf, &source_len_for_uncompress);
    strm.next_in = decomp_buf;
    strm.avail_in = (uInt)(decomp_capacity > 0 ? decomp_capacity : 0);
    strm.adler = crc_combined;
    strm.total_in = crcA;
    int rc_inflate_validate = inflateValidate(&strm, 15);
    gzFile gzf = gzopen("zlib_api_sequence_out.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (const void *)decomp_buf, (unsigned int)(decomp_capacity ? decomp_capacity : 0));
    int rc_gzflush = gzflush(gzf, 0);

    // step 4: Cleanup and validate tear-down
    int rc_inflate_end = inflateEnd(&strm);
    int rc_gzclose = gzclose(gzf);
    delete [] src;
    delete [] comp_buf;
    delete [] decomp_buf;
    (void)version;
    (void)crcA;
    (void)crcB;
    (void)crc_combined;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_uncompress;
    (void)rc_inflate_validate;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_inflate_end;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}