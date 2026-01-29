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
//<ID> 11
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *ver = zlibVersion();
    const Bytef source[] = "Example dictionary data for zlib usage.";
    uLong sourceLen = (uLong)(sizeof(source) - 1);
    uLong destCap = compressBound(sourceLen);
    Bytef *dest = new Bytef[destCap];
    uLongf destLen = (uLongf)destCap;
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_compress;
    int rc_inflate_init;
    int rc_setdict;
    int rc_inflate_end;
    int rc_gzclose;
    // step 2: Initialize
    rc_compress = compress(dest, &destLen, source, sourceLen);
    rc_inflate_init = inflateInit_(&strm, ver, (int)sizeof(z_stream));
    // step 3: Operate -> Validate
    rc_setdict = inflateSetDictionary(&strm, source, (uInt)sourceLen);
    gzprintf(gz, "compress=%d init=%d setdict=%d destLen=%lu\n", rc_compress, rc_inflate_init, rc_setdict, (unsigned long)destLen);
    // step 4: Cleanup
    rc_inflate_end = inflateEnd(&strm);
    rc_gzclose = gzclose(gz);
    delete [] dest;
    // API sequence test completed successfully
    return 66;
}