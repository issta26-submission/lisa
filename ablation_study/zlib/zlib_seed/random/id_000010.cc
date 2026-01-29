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
//<ID> 10
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
    const Bytef source[] = "example_dictionary_data";
    uLong sourceLen = (uLong)(sizeof(source) - 1);
    uLongf destLen = compressBound(sourceLen);
    Bytef *dest = (Bytef *)malloc((size_t)destLen);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_compress = compress(dest, &destLen, source, sourceLen);
    int rc_inflate_init = inflateInit_(&strm, ver, (int)sizeof(z_stream));
    // step 2: Configure
    strm.next_in = dest;
    strm.avail_in = (uInt)destLen;
    int rc_inflate_setdict = inflateSetDictionary(&strm, source, (uInt)sourceLen);
    // step 3: Operate -> Validate
    int rc_gzprintf = gzprintf(gz, "ver=%s rc_compress=%d rc_inflate_init=%d rc_inflate_setdict=%d destLen=%lu rc_gzprintf=%d\n",
                               ver, rc_compress, rc_inflate_init, rc_inflate_setdict, (unsigned long)destLen, rc_gzprintf);
    // step 4: Cleanup
    int rc_inflate_end = inflateEnd(&strm);
    int rc_gzclose = gzclose(gz);
    free(dest);
    // API sequence test completed successfully
    return 66;
}