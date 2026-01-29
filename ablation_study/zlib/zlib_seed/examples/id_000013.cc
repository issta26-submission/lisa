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
//<ID> 13
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const Bytef source[] = "The quick brown fox jumps over the lazy dog. zlib test data for compression.";
    uLong sourceLen = (uLong)(sizeof(source) - 1);
    uLongf destBound = (uLongf)compressBound(sourceLen);
    Bytef * dest = new Bytef[(size_t)destBound];
    uLongf destLen = destBound;
    gzFile gz;
    z_stream strm;
    const char * ver = zlibVersion();
    int rc_compress;
    int rc_gzprintf;
    int rc_init;
    int rc_setdict;
    int rc_inflateEnd;
    int rc_gzclose;

    // step 2: Setup / Operate (compress + log)
    rc_compress = compress(dest, &destLen, source, sourceLen);
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    rc_gzprintf = gzprintf(gz, "compress rc=%d destLen=%lu sourceLen=%lu\n", rc_compress, (unsigned long)destLen, (unsigned long)sourceLen);

    // step 3: Initialize -> Configure (inflate init and provide dictionary)
    memset(&strm, 0, sizeof(z_stream));
    rc_init = inflateInit_(&strm, ver, (int)sizeof(z_stream));
    strm.next_in = dest;
    strm.avail_in = (uInt)destLen;
    const Bytef * dict = source;
    uInt dictLen = (uInt)16;
    rc_setdict = inflateSetDictionary(&strm, dict, dictLen);

    // step 4: Validate -> Cleanup
    rc_inflateEnd = inflateEnd(&strm);
    rc_gzclose = gzclose(gz);
    delete [] dest;
    // API sequence test completed successfully
    return 66;
}