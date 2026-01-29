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
//<ID> 1457
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data and compressed buffer)
    const Bytef src[] = "zlib test data for inflate/deflate sequence";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(src_len);
    Bytef * comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (int)bound);
    uLongf compLen = (uLongf)bound;
    int comp_ret = compress(comp, &compLen, src, src_len);

    // step 2: Configure (initialize deflate/inflate streams and set gzip header, prepare inflateBack)
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    z_stream backstrm;
    memset(&backstrm, 0, (int)sizeof(backstrm));
    const char * version = zlibVersion();
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (int)sizeof(head));
    head.text = 0;
    int sethdr_ret = deflateSetHeader(&dstrm, &head);
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    unsigned char window[32];
    memset(window, 0, (int)sizeof(window));
    int back_init_ret = inflateBackInit_(&backstrm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Operate (attach compressed data to inflate stream and perform inflate)
    istrm.next_in = comp;
    istrm.avail_in = (uInt)compLen;
    uLong out_size = src_len + 64;
    Bytef * out = (Bytef *)malloc((size_t)out_size);
    memset(out, 0, (int)out_size);
    istrm.next_out = out;
    istrm.avail_out = (uInt)out_size;
    int infl_ret = inflate(&istrm, 0);

    // step 4: Validate & Cleanup (end streams, free buffers)
    int def_end_ret = deflateEnd(&dstrm);
    int inf_end_ret = inflateEnd(&istrm);
    int back_end_ret = inflateBackEnd(&backstrm);
    free(comp);
    free(out);
    (void)comp_ret;
    (void)def_init_ret;
    (void)sethdr_ret;
    (void)inf_init_ret;
    (void)back_init_ret;
    (void)infl_ret;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)back_end_ret;
    (void)version;
    (void)bound;
    (void)out_size;
    // API sequence test completed successfully
    return 66;
}