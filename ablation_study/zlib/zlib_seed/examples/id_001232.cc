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
//<ID> 1232
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payloadA[] = "Hello_zlib_Alpha";
    const char payloadB[] = "World_zlib_Beta";
    const uLong lenA = (uLong)(sizeof(payloadA) - 1);
    const uLong lenB = (uLong)(sizeof(payloadB) - 1);
    const uLong combinedLen = lenA + lenB;
    const char *version = zlibVersion();
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    Bytef *src = new Bytef[(size_t)combinedLen];
    memcpy(src, payloadA, (size_t)lenA);
    memcpy(src + lenA, payloadB, (size_t)lenB);
    uLongf compBound = compressBound(combinedLen);
    Bytef *comp_buf = new Bytef[(size_t)compBound];
    memset(comp_buf, 0, (size_t)compBound);
    uLongf compLen = compBound;
    uLongf decompLen = combinedLen + 16;
    Bytef *decomp_buf = new Bytef[(size_t)decompLen];
    memset(decomp_buf, 0, (size_t)decompLen);
    uLong crcA = crc32(0UL, (const Bytef *)payloadA, (uInt)lenA);
    uLong crcB = crc32(0UL, (const Bytef *)payloadB, (uInt)lenB);
    uLong combined_crc = crc32_combine(crcA, crcB, (off_t)lenB);

    // step 2: Setup (compress source and initialize inflate stream)
    int rc_comp = compress2(comp_buf, &compLen, src, (uLong)combinedLen, 6);
    int rc_inflate_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    strm.next_in = comp_buf;
    strm.avail_in = (uInt)compLen;
    strm.next_out = decomp_buf;
    strm.avail_out = (uInt)decompLen;

    // step 3: Core operations (validate inflate state, uncompress, write original via gz)
    int rc_inflate_validate = inflateValidate(&strm, 1);
    uLong sourceLenVar = (uLong)compLen;
    int rc_uncompress = uncompress2(decomp_buf, &decompLen, comp_buf, &sourceLenVar);
    gzFile gzf = gzopen("zlib_api_sequence_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (const void *)src, (unsigned int)(combinedLen ? combinedLen : 0));
    int rc_gzclose = gzclose(gzf);

    // step 4: Validate and cleanup
    int rc_inflate_end = inflateEnd(&strm);
    delete [] src;
    delete [] comp_buf;
    delete [] decomp_buf;
    (void)version;
    (void)rc_comp;
    (void)rc_inflate_init;
    (void)rc_inflate_validate;
    (void)rc_uncompress;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    (void)crcA;
    (void)crcB;
    (void)combined_crc;
    // API sequence test completed successfully
    return 66;
}