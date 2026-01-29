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
//<ID> 1104
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize and configure deflate stream, allocate buffers
    const Bytef source[] = "zlib API sequence payload demonstrating deflate -> crc32 -> inflateBackEnd.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&def_strm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    def_strm.next_in = (Bytef *)source;
    def_strm.avail_in = (uInt)source_len;
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)bound;

    // step 2: Operate - perform deflate to produce compressed output and finalize deflate
    int def_ret = deflate(&def_strm, 4);
    uLong comp_size = def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);

    // step 3: Validate - compute CRC32 over compressed output and prepare an inflate stream for backend cleanup
    uLong crc = crc32(0UL, comp_buf, (uInt)comp_size);
    z_stream infl_strm;
    memset(&infl_strm, 0, (int)sizeof(infl_strm));
    infl_strm.next_in = comp_buf;
    infl_strm.avail_in = (uInt)comp_size;
    int infl_back_end_ret = inflateBackEnd(&infl_strm);

    // step 4: Cleanup resources and finalize
    free(comp_buf);
    (void)version;
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)def_end_ret;
    (void)crc;
    (void)comp_size;
    (void)infl_back_end_ret;
    return 66;
    // API sequence test completed successfully
}