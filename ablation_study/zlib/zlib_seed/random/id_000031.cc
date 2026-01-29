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
//<ID> 31
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src_c[] = "Example zlib data to compress and use with dictionary";
    const char dict_c[] = "Example zlib dictionary content";
    const Bytef *source = (const Bytef *)src_c;
    const Bytef *dictionary = (const Bytef *)dict_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong dictLen = (uLong)(sizeof(dict_c) - 1);
    uInt dictLength = (uInt)dictLen;
    Bytef compBuf[1024];
    uLong bound = compressBound(sourceLen);
    uLongf compLen = (uLongf)sizeof(compBuf);
    int rc_compress = compress2(compBuf, &compLen, source, sourceLen, 6);

    // step 2: Setup -> Initialize inflate stream and associate buffers
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *ver = zlibVersion();
    int rc_inflate_init = inflateInit_(&strm, ver, (int)sizeof(z_stream));
    strm.next_in = compBuf;
    strm.avail_in = (uInt)compLen;
    Bytef outBuf[1024];
    strm.next_out = outBuf;
    strm.avail_out = (uInt)sizeof(outBuf);
    int rc_setdict = inflateSetDictionary(&strm, dictionary, dictLength);

    // step 3: Operate -> Validate and record status with gz APIs
    int rc_validate = inflateValidate(&strm, 1);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int gz_errnum = 0;
    const char *gz_errstr = gzerror(gz, &gz_errnum);
    gzprintf(gz, "bound=%lu rc_compress=%d compLen=%lu rc_inflate_init=%d rc_setdict=%d rc_validate=%d gz_err=\"%s\" errnum=%d\n",
             (unsigned long)bound, rc_compress, (unsigned long)compLen, rc_inflate_init, rc_setdict, rc_validate,
             gz_errstr ? gz_errstr : "NULL", gz_errnum);

    // step 4: Cleanup
    inflateEnd(&strm);
    int rc_close = gzclose(gz);
    (void)rc_close;
    (void)rc_compress;
    (void)rc_inflate_init;
    (void)rc_setdict;
    (void)rc_validate;
    (void)bound;
    (void)compLen;
    (void)gz_errstr;
    (void)gz_errnum;

    // API sequence test completed successfully
    return 66;
}