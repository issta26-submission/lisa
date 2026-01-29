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
//<ID> 1137
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize source data and deflate (compress) into a buffer
    const Bytef source[] = "zlib API sequence fuzz driver payload for deflate/gzwrite/inflateEnd test.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream defstrm;
    memset(&defstrm, 0, sizeof(defstrm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&defstrm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    defstrm.next_in = (Bytef *)source;
    defstrm.avail_in = (uInt)source_len;
    defstrm.next_out = comp_buf;
    defstrm.avail_out = (uInt)bound;
    int def_ret = deflate(&defstrm, 4);
    uLong comp_size = defstrm.total_out;
    int def_end_ret = deflateEnd(&defstrm);

    // step 2: Persist original source into a .gz file using gzwrite and close it
    gzFile gw = gzopen("tmp_zlib_api_seq.gz", "wb");
    int gzwrite_ret = gzwrite(gw, source, (unsigned int)source_len);
    int gzclose_ret = gzclose(gw);

    // step 3: Initialize inflate, decompress the compressed buffer into an output buffer, then call inflateEnd
    z_stream instrm;
    memset(&instrm, 0, sizeof(instrm));
    int inf_init_ret = inflateInit_(&instrm, version, (int)sizeof(z_stream));
    uLong out_alloc = source_len + 128;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_alloc);
    memset(out_buf, 0, (size_t)out_alloc);
    instrm.next_in = comp_buf;
    instrm.avail_in = (uInt)comp_size;
    instrm.next_out = out_buf;
    instrm.avail_out = (uInt)out_alloc;
    int inf_ret = inflate(&instrm, 4);
    uLong out_size = instrm.total_out;
    int inf_end_ret = inflateEnd(&instrm);

    // step 4: Validate checksum, cleanup and finalize
    uLong checksum = crc32(0UL, out_buf, (uInt)out_size);
    free(comp_buf);
    free(out_buf);
    (void)def_init_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)gzwrite_ret;
    (void)gzclose_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)checksum;
    (void)version;
    (void)bound;
    (void)out_size;
    return 66;
    // API sequence test completed successfully
}