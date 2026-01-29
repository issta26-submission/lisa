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
//<ID> 1239
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payloadA[] = "First_part_payload";
    const char payloadB[] = "Second_part_payload";
    const uLong lenA = (uLong)(sizeof(payloadA) - 1);
    const uLong lenB = (uLong)(sizeof(payloadB) - 1);
    const uLong srcLen = lenA + lenB;
    const char *version = zlibVersion();
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payloadA, (size_t)lenA);
    memcpy(src + lenA, payloadB, (size_t)lenB);

    // step 2: Setup (compress the combined source into a compact buffer)
    uLong bound = compressBound(srcLen);
    Bytef *compBuf = new Bytef[(size_t)bound];
    memset(compBuf, 0, (size_t)bound);
    uLongf compLen_f = (uLongf)bound;
    int rc_compress = compress2(compBuf, &compLen_f, src, srcLen, 6);
    uLong compLen_u = (uLong)compLen_f;
    uLong compLen_for_uncompress = compLen_u;

    // step 3: Core operations (uncompress into a destination buffer, validate inflate state, compute and combine CRCs, write compressed data to a gz file)
    uLongf decompLen_f = (uLongf)srcLen;
    Bytef *decompBuf = new Bytef[(size_t)decompLen_f];
    memset(decompBuf, 0, (size_t)decompLen_f);
    int rc_uncompress2 = uncompress2(decompBuf, &decompLen_f, compBuf, &compLen_for_uncompress);

    uLong crc1 = crc32(0UL, (const Bytef *)payloadA, (uInt)lenA);
    uLong crc2 = crc32(0UL, (const Bytef *)payloadB, (uInt)lenB);
    uLong combined_crc = crc32_combine(crc1, crc2, (off_t)lenB);

    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    istrm.next_in = decompBuf;
    istrm.avail_in = (uInt)decompLen_f;
    istrm.total_in = combined_crc;
    int rc_inflate_validate = inflateValidate(&istrm, 1);

    gzFile gzf = gzopen("zlib_api_sequence_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (const void *)compBuf, (unsigned int)(compLen_for_uncompress ? compLen_for_uncompress : 0));

    // step 4: Cleanup and finalization
    int rc_gzclose = gzclose(gzf);
    delete [] src;
    delete [] compBuf;
    delete [] decompBuf;
    (void)version;
    (void)rc_compress;
    (void)rc_uncompress2;
    (void)crc1;
    (void)crc2;
    (void)combined_crc;
    (void)rc_inflate_validate;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}