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
//<ID> 1233
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payloadA[] = "First segment of test payload";
    const char payloadB[] = "Second segment appended";
    const uLong lenA = (uLong)(sizeof(payloadA) - 1);
    const uLong lenB = (uLong)(sizeof(payloadB) - 1);
    const uLong totalLen = lenA + lenB;
    const char *version = zlibVersion();
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    Bytef *srcBuf = new Bytef[(size_t)totalLen];
    memset(srcBuf, 0, (size_t)totalLen);
    memcpy(srcBuf, payloadA, (size_t)lenA);
    memcpy(srcBuf + lenA, payloadB, (size_t)lenB);

    // step 2: Setup (compress source and initialize inflate stream)
    uLong compBound = compressBound(totalLen);
    Bytef *compBuf = new Bytef[(size_t)compBound];
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_compress = compress2(compBuf, &compLen, srcBuf, (uLong)totalLen, 6);
    uLong crcA = crc32(0UL, (const Bytef *)payloadA, (uInt)lenA);
    uLong crcB = crc32(0UL, (const Bytef *)payloadB, (uInt)lenB);
    uLong combined_crc = crc32_combine(crcA, crcB, (off_t)lenB);
    int rc_inflate_init = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)compLen;
    istrm.adler = combined_crc;

    // step 3: Core operations (decompress with uncompress2, validate stream, write output gz)
    Bytef *destBuf = new Bytef[(size_t)totalLen];
    memset(destBuf, 0, (size_t)totalLen);
    uLongf destLen = (uLongf)totalLen;
    uLong sourceLenVar = (uLong)compLen;
    int rc_uncompress = uncompress2(destBuf, &destLen, compBuf, &sourceLenVar);
    int rc_inflate_validate = inflateValidate(&istrm, 0);
    gzFile gzf = gzopen("zlib_api_sequence_output.gz", "wb+");
    int rc_gzwrite = gzwrite(gzf, (const void *)destBuf, (unsigned int)(destLen ? destLen : 0));
    int rc_gzflush = gzflush(gzf, 0);
    int rc_gzclose = gzclose(gzf);

    // step 4: Cleanup and validate
    int rc_inflate_end = inflateEnd(&istrm);
    delete [] srcBuf;
    delete [] compBuf;
    delete [] destBuf;
    (void)version;
    (void)rc_compress;
    (void)crcA;
    (void)crcB;
    (void)combined_crc;
    (void)rc_inflate_init;
    (void)rc_uncompress;
    (void)rc_inflate_validate;
    (void)sourceLenVar;
    (void)destLen;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}