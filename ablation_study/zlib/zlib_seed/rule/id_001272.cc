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
//<ID> 1272
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source, dictionary, and buffers; compute dictionary checksum
    const uLong SRC_LEN = 128UL;
    Bytef *src = (Bytef *)malloc((size_t)SRC_LEN);
    memset(src, 'A', (size_t)SRC_LEN);
    const uInt DICT_LEN = 32U;
    Bytef *dict = (Bytef *)malloc((size_t)DICT_LEN);
    memset(dict, 'D', (size_t)DICT_LEN);
    uLong dict_adler = adler32(0UL, dict, DICT_LEN);

    uLong comp_bound = compressBound(SRC_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)comp_bound);
    memset(comp, 0, (size_t)comp_bound);
    uLong decomp_len = SRC_LEN + 64;
    Bytef *decomp = (Bytef *)malloc((size_t)decomp_len);
    memset(decomp, 0, (size_t)decomp_len);

    // step 2: Initialize deflate, attach dictionary, compress source
    const char *ver = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    deflateSetDictionary(&dstrm, dict, DICT_LEN);
    dstrm.next_in = src;
    dstrm.avail_in = (uInt)SRC_LEN;
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)comp_bound;
    deflate(&dstrm, 4); /* Z_FINISH */
    uLong comp_size = dstrm.total_out;

    // step 3: Initialize inflate, provide same dictionary, inflate compressed data, call inflateBackEnd
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    inflateInit_(&istrm, ver, (int)sizeof(z_stream));
    inflateSetDictionary(&istrm, dict, DICT_LEN);
    istrm.next_in = comp;
    istrm.avail_in = (uInt)comp_size;
    istrm.next_out = decomp;
    istrm.avail_out = (uInt)decomp_len;
    inflate(&istrm, 4); /* Z_FINISH */
    inflateBackEnd(&istrm);

    // step 4: Use gzFile APIs to write original data and read it back, compute checksum; cleanup all resources
    gzFile gf = gzopen("test_zlib_api_sequence_tmp.gz", "wb");
    gzwrite(gf, src, (unsigned int)SRC_LEN);
    gzclose(gf);
    gzFile gfr = gzopen("test_zlib_api_sequence_tmp.gz", "rb");
    Bytef *readbuf = (Bytef *)malloc((size_t)SRC_LEN);
    memset(readbuf, 0, (size_t)SRC_LEN);
    int read_bytes = gzread(gfr, readbuf, (unsigned int)SRC_LEN);
    (void)read_bytes;
    gzclose(gfr);
    uLong adler_src = adler32(0UL, src, (uInt)SRC_LEN);
    uLong adler_read = adler32(0UL, readbuf, (uInt)SRC_LEN);

    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(src);
    free(dict);
    free(comp);
    free(decomp);
    free(readbuf);
    // API sequence test completed successfully
    return 66;
}