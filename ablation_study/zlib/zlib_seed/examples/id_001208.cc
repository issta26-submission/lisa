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
//<ID> 1208
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations
    const char payload[] = "zlib sequence payload for gzdopen/deflateCopy/adler32_z/inflateInit_/gzeof demonstration";
    uLong payload_len = (uLong)(sizeof(payload) - 1);
    const char *version = zlibVersion();
    z_stream def_src;
    z_stream def_dst;
    z_stream inf_strm;
    memset(&def_src, 0, (size_t)sizeof(z_stream));
    memset(&def_dst, 0, (size_t)sizeof(z_stream));
    memset(&inf_strm, 0, (size_t)sizeof(z_stream));

    // step 2: Setup (initialize deflate and inflate streams and copy deflate state)
    int rc_def_init = deflateInit_(&def_src, 6, version, (int)sizeof(z_stream));
    def_src.next_in = (Bytef *)payload;
    def_src.avail_in = (uInt)payload_len;
    int rc_defcopy = deflateCopy(&def_dst, &def_src);
    int rc_inf_init = inflateInit_(&inf_strm, version, (int)sizeof(z_stream));
    inf_strm.next_in = (Bytef *)payload;
    inf_strm.avail_in = (uInt)payload_len;

    // step 3: Core operations (compute Adler-32, open gz stream, write, check EOF)
    uLong adler = adler32_z(0UL, (const Bytef *)payload, (z_size_t)payload_len);
    gzFile gzf = gzdopen(1, "wb");
    int rc_gzwrite = gzwrite(gzf, (voidpc)payload, (unsigned int)payload_len);
    int eof_flag = gzeof(gzf);

    // step 4: Validate and cleanup
    int rc_gzclose = gzclose(gzf);
    int rc_def_end_dst = deflateEnd(&def_dst);
    int rc_def_end_src = deflateEnd(&def_src);
    int rc_inf_end = inflateEnd(&inf_strm);
    (void)version;
    (void)rc_def_init;
    (void)rc_defcopy;
    (void)rc_inf_init;
    (void)adler;
    (void)rc_gzwrite;
    (void)eof_flag;
    (void)rc_gzclose;
    (void)rc_def_end_dst;
    (void)rc_def_end_src;
    (void)rc_inf_end;
    // API sequence test completed successfully
    return 66;
}