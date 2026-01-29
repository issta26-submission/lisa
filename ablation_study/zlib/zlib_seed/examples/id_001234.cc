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
//<ID> 1234
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payloadA[] = "Hello_zlib_A";
    const char payloadB[] = "World_zlib_B";
    const uLong lenA = (uLong)(sizeof(payloadA) - 1);
    const uLong lenB = (uLong)(sizeof(payloadB) - 1);
    const char combined_src[] = "Hello_zlib_A" "World_zlib_B";
    const uLong lenCombined = (uLong)(sizeof(combined_src) - 1);
    const char *version = zlibVersion();
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    uLong crcA = crc32(0UL, (const Bytef *)payloadA, (uInt)lenA);
    uLong crcB = crc32(0UL, (const Bytef *)payloadB, (uInt)lenB);

    // step 2: Setup (compress combined source into a buffer)
    uLong compBound = compressBound(lenCombined);
    unsigned char *compBuf = new unsigned char[(size_t)compBound];
    memset(compBuf, 0, (size_t)compBound);
    uLongf compLen = (uLongf)compBound;
    int rc_compress = compress2((Bytef *)compBuf, &compLen, (const Bytef *)combined_src, (uLong)lenCombined, 6);

    // step 3: Core operations (uncompress back, validate inflate stream, compute combined crc and write to gz)
    uLongf destLen = (uLongf)(lenCombined + 16);
    unsigned char *destBuf = new unsigned char[(size_t)destLen];
    memset(destBuf, 0, (size_t)destLen);
    uLong sourceLenVar = (uLong)compLen;
    int rc_uncompress2 = uncompress2((Bytef *)destBuf, &destLen, (const Bytef *)compBuf, &sourceLenVar);

    istrm.next_in = (Bytef *)destBuf;
    istrm.avail_in = (uInt)(destLen ? destLen : 0);
    istrm.next_out = (Bytef *)compBuf;
    istrm.avail_out = (uInt)(compBound ? compBound : 0);
    int rc_inflate_validate = inflateValidate(&istrm, 1);

    uLong combined_crc = crc32_combine(crcA, crcB, (off_t)lenB);

    gzFile gzf = gzopen("zlib_api_sequence_output.gz", "wb+");
    int rc_gzwrite = gzwrite(gzf, (const void *)combined_src, (unsigned int)(lenCombined ? lenCombined : 0));
    int rc_gzclose = gzclose(gzf);

    // step 4: Cleanup and finalization
    delete [] compBuf;
    delete [] destBuf;
    (void)version;
    (void)rc_compress;
    (void)rc_uncompress2;
    (void)rc_inflate_validate;
    (void)rc_gzwrite;
    (void)crcA;
    (void)crcB;
    (void)combined_crc;
    (void)rc_gzclose;
    // API sequence test completed successfully
    return 66;
}