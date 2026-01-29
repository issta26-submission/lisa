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
//<ID> 15
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
    Bytef src[] = "Hello zlib sample data";
    uLong srcLen = (uLong)(sizeof(src) - 1);
    Bytef dict[] = { 'd', 'i', 'c', 't' };
    uInt dictLen = 4;
    Bytef dest[256];
    uLongf destLen = (uLongf)sizeof(dest);
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    int rc_init;
    int rc_setdict;
    int rc_compress;
    int rc_inflateEnd;

    // step 2: Initialize -> Configure
    rc_init = inflateInit_(&strm, ver, (int)sizeof(z_stream));
    rc_setdict = inflateSetDictionary(&strm, dict, dictLen);

    // step 3: Operate -> Validate
    rc_compress = compress(dest, &destLen, src, srcLen);
    gzprintf(gz, "ver=%s init=%d setdict=%d compress=%d srcLen=%lu destLen=%lu\n",
             ver, rc_init, rc_setdict, rc_compress, (unsigned long)srcLen, (unsigned long)destLen);

    // step 4: Cleanup
    rc_inflateEnd = inflateEnd(&strm);
    gzclose(gz);
    // API sequence test completed successfully
    return 66;
}