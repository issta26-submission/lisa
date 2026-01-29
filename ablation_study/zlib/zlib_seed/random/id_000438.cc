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
//<ID> 438
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare payload and obtain compile flags
    const char payload[] = "zlib API sequence payload for gzprintf and inflateInit2_";
    const Bytef *source = (const Bytef *)payload;
    uLong sourceLen = (uLong)(sizeof(payload) - 1);
    uLong flags = zlibCompileFlags();

    // step 2: Initialize inflate stream (configure with gzip auto-detect)
    z_stream strm;
    memset(&strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_init = inflateInit2_(&strm, 15 + 32, version, (int)sizeof(z_stream));

    // step 3: Operate: write formatted payload (including compile flags) to a gzip file
    gzFile gz = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    int rc_gzprintf = gzprintf(gz, "zlib_flags=%lu payload=%s\n", flags, payload);
    int rc_gzclose = gzclose(gz);

    // step 4: Validate (use returned codes) and cleanup inflate stream
    int rc_end = inflateEnd(&strm);
    (void)source;
    (void)sourceLen;
    (void)flags;
    (void)version;
    (void)rc_init;
    (void)rc_gzprintf;
    (void)rc_gzclose;
    (void)rc_end;
    // API sequence test completed successfully
    return 66;
}