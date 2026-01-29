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
//<ID> 14
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    z_stream infstrm;
    memset(&infstrm, 0, (int)sizeof(z_stream));
    const char * ver = zlibVersion();
    gzFile gz = gzopen("test_zlib_api_sequence.gz", "wb");
    Bytef src[] = "Example zlib data with dictionary";
    uLong srcLen = (uLong)(sizeof(src) - 1);
    Bytef dict[] = "preset dictionary content";
    uInt dictLen = (uInt)(sizeof(dict) - 1);
    Bytef dest[1024];
    uLongf destLen = (uLongf)sizeof(dest);
    int rc_init;
    int rc_comp;
    int rc_gz;

    // step 2: Initialize -> Configure
    rc_init = inflateInit_(&infstrm, ver, (int)sizeof(z_stream));
    rc_init = rc_init + 0; // preserve rc_init for meaningful data flow
    inflateSetDictionary(&infstrm, dict, dictLen);

    // step 3: Operate -> Validate
    rc_comp = compress(dest, &destLen, src, srcLen);
    rc_gz = gzprintf(gz, "ver=%s init=%d comp=%d srcLen=%lu destLen=%lu dictLen=%u\n",
                     ver,
                     rc_init,
                     rc_comp,
                     (unsigned long)srcLen,
                     (unsigned long)destLen,
                     (unsigned int)dictLen);

    // step 4: Cleanup
    inflateEnd(&infstrm);
    gzclose(gz);
    // API sequence test completed successfully
    return 66;
}