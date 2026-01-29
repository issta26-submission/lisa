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
//<ID> 1098
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize / Configure deflate and compress a payload
    const char *version = zlibVersion();
    const Bytef source[] = "zlib API sequence payload to compress, inflate, and exercise gzgetc_ and gzclose_r.";
    uLong source_len = (uLong)(sizeof(source) - 1);
    z_stream defstrm;
    memset(&defstrm, 0, (int)sizeof(defstrm));
    int def_init_ret = deflateInit_(&defstrm, 6, version, (int)sizeof(z_stream));
    uLong bound = deflateBound(&defstrm, source_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (size_t)bound);
    defstrm.next_in = (Bytef *)source;
    defstrm.avail_in = (uInt)source_len;
    defstrm.next_out = comp_buf;
    defstrm.avail_out = (uInt)bound;
    int def_ret = deflate(&defstrm, 4); // Z_FINISH
    uLong comp_size = defstrm.total_out;
    int def_end_ret = deflateEnd(&defstrm);

    // step 2: Initialize inflate, set compressed data as input, and inflate to output buffer
    z_stream infstrm;
    memset(&infstrm, 0, (int)sizeof(infstrm));
    int inf_init_ret = inflateInit_(&infstrm, version, (int)sizeof(z_stream));
    infstrm.next_in = comp_buf;
    infstrm.avail_in = (uInt)comp_size;
    uLong out_buf_len = source_len + 128;
    Bytef *out_buf = (Bytef *)malloc((size_t)out_buf_len);
    memset(out_buf, 0, (size_t)out_buf_len);
    infstrm.next_out = out_buf;
    infstrm.avail_out = (uInt)out_buf_len;
    int inf_ret = inflate(&infstrm, 0); // Z_NO_FLUSH
    uLong inflated_size = infstrm.total_out;
    int inf_end_ret = inflateEnd(&infstrm);

    // step 3: Write compressed data to a .gz file, reposition, read one byte using gzgetc_, then close for reading
    gzFile gzf = gzopen("tmp_zlib_api_seq.gz", "wb+");
    int gzwrite_ret = gzwrite(gzf, (voidpc)comp_buf, (unsigned int)comp_size);
    off_t seek_ret = gzseek(gzf, (off_t)0, 0); // rewind to start
    int first_byte = gzgetc_(gzf);
    int gzclose_r_ret = gzclose_r(gzf);

    // step 4: Validate simple checksum and cleanup
    uLong crc = crc32(0UL, comp_buf, (uInt)comp_size);
    free(comp_buf);
    free(out_buf);
    (void)def_init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_size;
    (void)def_end_ret;
    (void)inf_init_ret;
    (void)inf_ret;
    (void)inflated_size;
    (void)inf_end_ret;
    (void)gzwrite_ret;
    (void)seek_ret;
    (void)first_byte;
    (void)gzclose_r_ret;
    (void)crc;
    (void)version;
    (void)source_len;
    // API sequence test completed successfully
    return 66;
}