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
//<ID> 1231
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payloadA[] = "First_part_of_data";
    const char payloadB[] = "Second_part_of_data";
    const uLong lenA = (uLong)(sizeof(payloadA) - 1);
    const uLong lenB = (uLong)(sizeof(payloadB) - 1);
    const uLong srcLen = lenA + lenB;
    const char *version = zlibVersion();
    z_stream strm;
    memset(&strm, 0, (size_t)sizeof(z_stream));
    unsigned char *sourceBuf = new unsigned char[(size_t)srcLen];
    unsigned char *compBuf = NULL;
    unsigned char *decompBuf = NULL;
    memset(sourceBuf, 0, (size_t)srcLen);

    // step 2: Setup (assemble source and allocate compression buffer)
    memcpy(sourceBuf, payloadA, (size_t)lenA);
    memcpy(sourceBuf + lenA, payloadB, (size_t)lenB);
    uLongf compBufCap = (uLongf)compressBound((uLong)srcLen);
    compBuf = new unsigned char[(size_t)compBufCap];
    memset(compBuf, 0, (size_t)compBufCap);
    uLongf compLen_f = compBufCap;
    int rc_compress = compress2((Bytef *)compBuf, &compLen_f, (const Bytef *)sourceBuf, (uLong)srcLen, 6);
    uLong compLen = (uLong)compLen_f;

    // step 3: Core operations (checksums, inflate init/validate, gzip write, uncompress2)
    uLong crcA = crc32(0UL, (const Bytef *)payloadA, (uInt)lenA);
    uLong crcB = crc32(0UL, (const Bytef *)payloadB, (uInt)lenB);
    uLong combined_crc = crc32_combine(crcA, crcB, (off_t)lenB);
    int rc_inflate_init = inflateInit_(&strm, version, (int)sizeof(z_stream));
    int rc_inflate_validate = inflateValidate(&strm, 0);
    gzFile gzf = gzopen("zlib_api_seq_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (const void *)compBuf, (unsigned int)(compLen ? compLen : 0));
    int rc_gzclose = gzclose(gzf);
    decompBuf = new unsigned char[(size_t)srcLen];
    memset(decompBuf, 0, (size_t)srcLen);
    uLongf decompLen_f = (uLongf)srcLen;
    uLong compLen_for_uncompress = compLen;
    int rc_uncompress2 = uncompress2((Bytef *)decompBuf, &decompLen_f, (const Bytef *)compBuf, &compLen_for_uncompress);

    // step 4: Validate and cleanup
    strm.total_in = combined_crc;
    int rc_inflate_end = inflateEnd(&strm);
    delete [] sourceBuf;
    delete [] compBuf;
    delete [] decompBuf;
    (void)version;
    (void)rc_compress;
    (void)crcA;
    (void)crcB;
    (void)combined_crc;
    (void)rc_inflate_init;
    (void)rc_inflate_validate;
    (void)rc_gzwrite;
    (void)rc_gzclose;
    (void)rc_uncompress2;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}