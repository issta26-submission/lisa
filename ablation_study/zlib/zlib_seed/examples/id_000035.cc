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
//<ID> 35
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & Initialize
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char src_c[] = "Example zlib dictionary and payload data";
    const Bytef *source = (const Bytef *)src_c;
    uLong sourceLen = (uLong)(sizeof(src_c) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *compressed = (Bytef *)malloc((size_t)bound);
    uLongf compressedLen = (uLongf)bound;
    int rc_compress;
    int rc_inflateInit;
    int rc_setdict;
    int rc_validate;
    gzFile gz;
    int rc_gzwrite;
    int gz_errnum;
    const char *gz_errstr;

    // step 2: Setup & Configure (compress data, init inflate, set dictionary)
    rc_compress = compress2(compressed, &compressedLen, source, sourceLen, 6);
    rc_inflateInit = inflateInit_(&strm, zlibVersion(), (int)sizeof(z_stream));
    rc_setdict = inflateSetDictionary(&strm, source, (uInt)sourceLen);
    rc_validate = inflateValidate(&strm, 1);

    // step 3: Operate & Validate (write compressed output and query gzip error)
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    rc_gzwrite = gzwrite(gz, compressed, (unsigned int)compressedLen);
    gz_errstr = gzerror(gz, &gz_errnum);
    gzprintf(gz, "compress=%d inflateInit=%d setDict=%d validate=%d gzwrite=%d gzerrnum=%d gzerr=\"%s\"\n",
             rc_compress, rc_inflateInit, rc_setdict, rc_validate, rc_gzwrite, gz_errnum, gz_errstr ? gz_errstr : "null");

    // step 4: Cleanup
    inflateEnd(&strm);
    gzclose(gz);
    free(compressed);

    // API sequence test completed successfully
    return 66;
}