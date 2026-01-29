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
//<ID> 1105
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize - prepare source data and deflate stream
    const Bytef source[] = "zlib API sequence payload: initialize -> operate -> validate -> cleanup.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream deflate_strm;
    memset(&deflate_strm, 0, (int)sizeof(deflate_strm));
    const char *ver = zlibVersion();
    int def_init_ret = deflateInit_(&deflate_strm, 6, ver, (int)sizeof(z_stream));

    // step 2: Configure - allocate output buffer and wire the stream
    uLong out_alloc = source_len + 128UL;
    Bytef *comp_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(comp_buf, 0, (size_t)out_alloc);
    deflate_strm.next_in = (Bytef *)source;
    deflate_strm.avail_in = (uInt)source_len;
    deflate_strm.next_out = comp_buf;
    deflate_strm.avail_out = (uInt)out_alloc;

    // step 3: Operate - perform deflate, finalize stream, and validate with CRC32
    int def_ret = deflate(&deflate_strm, 4);
    uLong comp_size = deflate_strm.total_out;
    int def_end_ret = deflateEnd(&deflate_strm);
    uLong comp_crc = crc32(0UL, comp_buf, (uInt)comp_size);
    (void)def_init_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)comp_crc;

    // step 4: Cleanup - initialize an inflate stream and call inflateBackEnd, then free buffers
    z_stream inflate_strm;
    memset(&inflate_strm, 0, (int)sizeof(inflate_strm));
    int inf_init_ret = inflateInit_(&inflate_strm, ver, (int)sizeof(z_stream));
    int inf_back_end_ret = inflateBackEnd(&inflate_strm);
    free(comp_buf);
    (void)inf_init_ret;
    (void)inf_back_end_ret;
    // API sequence test completed successfully
    return 66;
}