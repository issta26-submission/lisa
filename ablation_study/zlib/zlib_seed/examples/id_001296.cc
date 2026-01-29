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
//<ID> 1296
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib payload for gzprintf and deflateEnd";
    const uLong srcLen = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    uLong compile_flags = zlibCompileFlags();
    Bytef *src = new Bytef[(size_t)srcLen];
    memset(src, 0, (size_t)srcLen);
    memcpy(src, payload, (size_t)srcLen);
    uLong bound = compressBound(srcLen);
    Bytef *out = new Bytef[(size_t)bound];
    memset(out, 0, (size_t)bound);
    z_stream dstrm;
    memset(&dstrm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (initialize deflate stream and attach buffers)
    int rc_deflate_init = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)srcLen;
    dstrm.next_out = out;
    dstrm.avail_out = (uInt)bound;

    // step 3: Core operations (open gzip file and write informational strings)
    gzFile gzf = gzopen("zlib_api_seq_out.gz", "wb");
    int rc_gzprintf_ver = gzprintf(gzf, version);
    int rc_gzprintf_payload = gzprintf(gzf, payload);

    // step 4: Cleanup and finalization
    int rc_gzclose = gzclose(gzf);
    int rc_deflate_end = deflateEnd(&dstrm);
    delete [] src;
    delete [] out;
    (void)compile_flags;
    (void)rc_deflate_init;
    (void)rc_gzprintf_ver;
    (void)rc_gzprintf_payload;
    (void)rc_gzclose;
    (void)rc_deflate_end;
    // API sequence test completed successfully
    return 66;
}