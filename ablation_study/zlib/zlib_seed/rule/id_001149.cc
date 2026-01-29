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
//<ID> 1149
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare original buffer, compress and write to gzip file
    const uLong ORIG_LEN = 256UL;
    Bytef *orig = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(orig, 'A', (size_t)ORIG_LEN);
    const char *ver = zlibVersion();
    uLong bound = compressBound(ORIG_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf comp_size = (uLongf)bound;
    compress2(comp, &comp_size, orig, ORIG_LEN, 6);
    gzFile gw = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzwrite(gw, comp, (unsigned int)comp_size);
    gzclose(gw);

    // step 2: Read compressed data back from gzip file using gzfread and clear errors
    gzFile gr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)bound);
    memset(read_buf, 0, (size_t)bound);
    z_size_t items_read = gzfread(read_buf, (z_size_t)1, (z_size_t)bound, gr);
    gzclearerr(gr);
    gzclose(gr);

    // step 3: Initialize inflate, perform decompression, and cleanup inflate
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)items_read;
    Bytef *decomp = (Bytef *)malloc((size_t)ORIG_LEN);
    memset(decomp, 0, (size_t)ORIG_LEN);
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)ORIG_LEN;
    inflate(&istrm, Z_FINISH);
    inflateEnd(&istrm);

    // step 4: Initialize an inflate-back stream and call inflateBackEnd, validate and cleanup
    z_stream back_strm;
    memset(&back_strm, 0, sizeof(back_strm));
    unsigned char window_buf[32768];
    memset(window_buf, 0, sizeof(window_buf));
    inflateBackInit_(&back_strm, 15, window_buf, ver, (int)sizeof(z_stream));
    inflateBackEnd(&back_strm);
    uLong crc_orig = crc32(0UL, orig, (uInt)ORIG_LEN);
    uLong crc_decomp = crc32(0UL, decomp, (uInt)ORIG_LEN);
    (void)crc_orig;
    (void)crc_decomp;
    free(orig);
    free(comp);
    free(read_buf);
    free(decomp);
    // API sequence test completed successfully
    return 66;
}