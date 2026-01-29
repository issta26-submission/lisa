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
//<ID> 973
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations and initial checksum of payload
    const char payload[] = "zlib API sequence payload exercising adler32_z, inflateInit_, gzopen, gzprintf and gzoffset64";
    const uLong payload_len = (uLong)(sizeof(payload) - 1);
    const Bytef *src_buf = (const Bytef *)payload;
    uLong adler_initial = adler32_z(0UL, src_buf, (z_size_t)payload_len);

    // step 2: Initialize inflate stream
    z_stream istrm;
    memset(&istrm, 0, (size_t)sizeof(z_stream));
    int rc_inflate_init = inflateInit_(&istrm, zlibVersion(), (int)sizeof(z_stream));

    // step 3: Operate on gzFile: open, write formatted data, query offset, close
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "w+b");
    int rc_gzprintf = gzprintf(gf, payload);
    off64_t offset_after_write = gzoffset64(gf);
    int rc_gzclose = gzclose(gf);

    // step 4: Cleanup inflate stream and finalize validation
    int rc_inflate_end = inflateEnd(&istrm);

    (void)adler_initial;
    (void)rc_inflate_init;
    (void)rc_gzprintf;
    (void)offset_after_write;
    (void)rc_gzclose;
    (void)rc_inflate_end;
    // API sequence test completed successfully
    return 66;
}