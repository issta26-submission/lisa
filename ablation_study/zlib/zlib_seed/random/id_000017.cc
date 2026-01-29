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
//<ID> 17
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
    const char * ver = zlibVersion();
    Bytef dict[] = "preset_dictionary";
    uInt dictLen = (uInt)(sizeof(dict) - 1);
    Bytef source[] = "Hello zlib dictionary example";
    uLong sourceLen = (uLong)(sizeof(source) - 1);
    Bytef dest[1024];
    uLongf destLen = (uLongf)sizeof(dest);
    int rc_comp;
    int rc_init;
    int rc_dict;
    int rc_printf;
    int rc_end;
    gzFile gz;
    // step 2: Initialize -> Configure
    rc_init = inflateInit_(&strm, ver, (int)sizeof(z_stream));
    rc_dict = inflateSetDictionary(&strm, dict, dictLen);
    // step 3: Operate -> Validate
    rc_comp = compress(dest, &destLen, source, sourceLen);
    gz = gzopen("test_zlib_api_sequence.gz", "wb");
    rc_printf = gzprintf(gz, "ver=%s comp=%d init=%d dict=%d outlen=%lu\n", ver, rc_comp, rc_init, rc_dict, (unsigned long)destLen);
    // step 4: Cleanup
    rc_end = inflateEnd(&strm);
    gzclose(gz);
    // API sequence test completed successfully
    return 66;
}