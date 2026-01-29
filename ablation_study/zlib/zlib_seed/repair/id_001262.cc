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
//<ID> 1262
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize version and deflate stream
    const Bytef src[] = "zlib API sequence payload: compress then store and read back via gz APIs";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream strm;
    memset(&strm, 0, (int)sizeof(strm));
    int init_ret = deflateInit_(&strm, 6, version, (int)sizeof(z_stream));

    // step 2: Configure buffers and perform deflate
    uLong bound = deflateBound(&strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    strm.next_in = (Bytef *)src;
    strm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    strm.next_out = comp_buf;
    strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    int def_ret = deflate(&strm, 4);
    uLong comp_size = strm.total_out;

    // step 3: Write compressed data and a textual footer into a gzip file, then close
    gzFile gzout = gzopen("test_zlib_api_sequence.gz", "wb");
    int gz_write_ret = gzwrite(gzout, comp_buf, (unsigned int)(comp_size > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_size));
    int gz_puts_ret = gzputs(gzout, "<<<footer>>>"); 
    int gzclose_ret1 = gzclose(gzout);

    // step 4: Reopen for reading, seek to start, read first byte, cleanup and finalize
    gzFile gzin = gzopen("test_zlib_api_sequence.gz", "rb");
    off_t seek_pos = gzseek(gzin, (off_t)0, 0);
    int first_char = gzgetc(gzin);
    int gzclose_ret2 = gzclose(gzin);
    int def_end_ret = deflateEnd(&strm);
    free(comp_buf);
    (void)version;
    (void)init_ret;
    (void)bound;
    (void)def_ret;
    (void)comp_size;
    (void)gz_write_ret;
    (void)gz_puts_ret;
    (void)gzclose_ret1;
    (void)seek_pos;
    (void)first_char;
    (void)gzclose_ret2;
    (void)def_end_ret;
    // API sequence test completed successfully
    return 66;
}