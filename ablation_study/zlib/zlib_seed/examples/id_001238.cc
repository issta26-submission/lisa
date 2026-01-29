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
//<ID> 1238
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payloadA[] = "Zlib_Part_A";
    const char payloadB[] = "Zlib_Part_B";
    const uLong lenA = (uLong)(sizeof(payloadA) - 1);
    const uLong lenB = (uLong)(sizeof(payloadB) - 1);
    const uLong srcLenTotal = lenA + lenB;
    const char *version = zlibVersion();
    z_stream zstr;
    memset(&zstr, 0, (size_t)sizeof(z_stream));
    unsigned char *src_buf = new unsigned char[(size_t)srcLenTotal];
    memcpy(src_buf, payloadA, (size_t)lenA);
    memcpy(src_buf + lenA, payloadB, (size_t)lenB);
    uLong comp_bound = compressBound(srcLenTotal);
    unsigned char *comp_buf = new unsigned char[(size_t)comp_bound];
    uLongf compLen = (uLongf)comp_bound;

    // step 2: Setup (compress source into comp_buf and prepare destination)
    int rc_compress = compress2((Bytef *)comp_buf, &compLen, (const Bytef *)src_buf, srcLenTotal, 6);
    uLongf destBufLen = srcLenTotal;
    unsigned char *dest_buf = new unsigned char[(size_t)destBufLen];
    uLong compLen_for_un = (uLong)compLen;

    // step 3: Core operations (uncompress, combine CRCs, validate inflate state, write gz)
    int rc_uncompress = uncompress2((Bytef *)dest_buf, &destBufLen, (const Bytef *)comp_buf, &compLen_for_un);
    uLong crcA = crc32(0UL, (const Bytef *)payloadA, (uInt)lenA);
    uLong crcB = crc32(0UL, (const Bytef *)payloadB, (uInt)lenB);
    uLong combined_crc = crc32_combine(crcA, crcB, (off_t)lenB);
    zstr.adler = combined_crc;
    int rc_inflate_validate = inflateValidate(&zstr, 1);
    gzFile gzf = gzopen("zlib_api_sequence_output.gz", "wb");
    int rc_gzwrite = gzwrite(gzf, (const void *)dest_buf, (unsigned int)(destBufLen ? destBufLen : 0));
    int rc_gzflush = gzflush(gzf, 0);
    int rc_gzclose = gzclose(gzf);

    // step 4: Cleanup and validation
    int rc_inflate_end = inflateEnd(&zstr);
    delete [] src_buf;
    delete [] comp_buf;
    delete [] dest_buf;
    (void)version;
    (void)rc_compress;
    (void)rc_uncompress;
    (void)crcA;
    (void)crcB;
    (void)combined_crc;
    (void)rc_inflate_validate;
    (void)rc_gzwrite;
    (void)rc_gzflush;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}