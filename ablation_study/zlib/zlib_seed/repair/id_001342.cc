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
//<ID> 1342
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data)
    const Bytef src[] = "Example payload for zlib API sequence: compress then write and read back via gz* APIs.";
    uLong src_len = (uLong)(sizeof(src) - 1);
    Bytef *src_buf = (Bytef *)malloc((size_t)src_len);
    memcpy(src_buf, src, (size_t)src_len);
    const char *version = zlibVersion();

    // step 2: Configure (initialize deflate stream and output buffer)
    z_stream def_strm;
    memset(&def_strm, 0, (int)sizeof(def_strm));
    int init_ret = deflateInit_(&def_strm, 6, version, (int)sizeof(z_stream));
    def_strm.next_in = src_buf;
    def_strm.avail_in = (uInt)src_len;
    uLong bound = deflateBound(&def_strm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    def_strm.next_out = comp_buf;
    def_strm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);

    // step 3: Operate (perform deflate, close deflate, write compressed data to a gz file)
    int def_ret = deflate(&def_strm, 4);
    uLong comp_len = (uLong)def_strm.total_out;
    int def_end_ret = deflateEnd(&def_strm);
    gzFile gz_write = gzopen("test_zlib_api_sequence.gz", "wb");
    int gw = gzwrite(gz_write, comp_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int gcw = gzclose(gz_write);

    // step 4: Validate & Cleanup (read back the gz file, seek, check EOF, compute a checksum, free resources)
    gzFile gz_read = gzopen("test_zlib_api_sequence.gz", "rb");
    uInt read_buf_size = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len);
    Bytef *read_buf = (Bytef *)malloc((size_t)(read_buf_size + 16));
    memset(read_buf, 0, (int)(read_buf_size + 16));
    int r1 = gzread(gz_read, read_buf, read_buf_size);
    int eof_before = gzeof(gz_read);
    off_t pos = gzseek(gz_read, 0, 0);
    int r2 = gzread(gz_read, read_buf, read_buf_size);
    int eof_after = gzeof(gz_read);
    int gcr = gzclose(gz_read);

    uLong crc1 = crc32(0L, read_buf, (uInt)(r1 > 0 ? r1 : 0));
    free(src_buf);
    free(comp_buf);
    free(read_buf);

    (void)init_ret;
    (void)def_ret;
    (void)def_end_ret;
    (void)gw;
    (void)gcw;
    (void)r1;
    (void)eof_before;
    (void)pos;
    (void)r2;
    (void)eof_after;
    (void)gcr;
    (void)crc1;
    (void)version;
    // API sequence test completed successfully
    return 66;
}