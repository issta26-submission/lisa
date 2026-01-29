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
//<ID> 30
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    const char src_c[] = "Example data for zlib compress and dictionary usage";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef compBuf[4096];
    uLongf compLen = (uLongf)sizeof(compBuf);
    int rc_compress = compress(compBuf, &compLen, source, sourceLen);
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *ver = zlibVersion();
    int rc_inflateInit = inflateInit_(&strm, ver, (int)sizeof(z_stream));

    // step 2: Configure
    const Bytef *dictionary = source;
    uInt dictLength = (uInt)sourceLen;
    int rc_inflateSetDict = inflateSetDictionary(&strm, dictionary, dictLength);
    int rc_validate = inflateValidate(&strm, 0);

    // step 3: Operate -> Validate
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    gzprintf(gz, "ver=%s bound=%lu compLen=%lu inflateInit=%d setDict=%d validate=%d\n",
             ver, (unsigned long)bound, (unsigned long)compLen, rc_inflateInit, rc_inflateSetDict, rc_validate);
    int gz_errnum = 0;
    const char *gz_errstr = gzerror(gz, &gz_errnum);

    // step 4: Cleanup
    inflateEnd(&strm);
    int rc_gzclose = gzclose(gz);
    (void)rc_compress; (void)rc_gzclose; (void)gz_errstr; (void)gz_errnum; (void)rc_validate; (void)bound;

    // API sequence test completed successfully
    return 66;
}