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
//<ID> 884
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const char *version = zlibVersion();
    const Bytef source[] = "Example payload for zlib compression and gzip writing.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream strm;
    memset(&strm, 0, sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));
    uLong src_adler = adler32(0UL, source, (uInt)source_len);

    // step 2: Configure
    strm.next_in = (Bytef *)source;
    strm.avail_in = (uInt)source_len;
    uLong bound = deflateBound(&strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)bound;

    // step 3: Operate
    int def_ret = deflate(&strm, 4);
    uLong comp_len = strm.total_out;
    uLong comp_adler = adler32(0UL, comp_buf, (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len));
    gzFile gz = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int gz_wr = gzwrite(gz, comp_buf, (unsigned int)comp_len);
    int gz_cl = gzclose(gz);

    // step 4: Validate / Cleanup
    int def_end = deflateEnd(&strm);
    free(comp_buf);
    (void)version;
    (void)init_ret;
    (void)src_adler;
    (void)bound;
    (void)def_ret;
    (void)comp_len;
    (void)comp_adler;
    (void)gz_wr;
    (void)gz_cl;
    (void)def_end;
    // API sequence test completed successfully
    return 66;
}