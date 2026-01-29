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
//<ID> 1452
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source data and compress it using compress2)
    const Bytef src[] = "zlib API sequence: prepare, compress, init streams, inflate, cleanup";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(src_len);
    Bytef * comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    uLong comp_len = bound;
    int comp_ret = compress2(comp_buf, &comp_len, src, src_len, 6);

    // step 2: Configure (initialize deflate, inflate and inflate-back streams and set gzip header)
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    z_stream backstrm;
    memset(&backstrm, 0, (int)sizeof(backstrm));
    const char * version = zlibVersion();
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    unsigned char * window = (unsigned char *)malloc((size_t)32768);
    memset(window, 0, 32768);
    int inf_back_init_ret = inflateBackInit_(&backstrm, 15, window, version, (int)sizeof(z_stream));
    gz_header header;
    memset(&header, 0, (int)sizeof(header));
    header.name = (Bytef *)"test-name";
    header.comment = (Bytef *)"test-comment";
    header.text = 0;
    header.hcrc = 0;
    int def_set_hdr_ret = deflateSetHeader(&dstrm, &header);

    // step 3: Operate (attach compressed data to inflate stream and perform a single inflate call)
    Bytef * out_buf = (Bytef *)malloc((size_t)(src_len + 32));
    memset(out_buf, 0, (int)(src_len + 32));
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)comp_len;
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)(src_len + 32);
    int inf_ret = inflate(&istrm, 0);

    // step 4: Validate & Cleanup (end streams, free buffers)
    int def_end_ret = deflateEnd(&dstrm);
    int inf_end_ret = inflateEnd(&istrm);
    int inf_back_end_ret = inflateBackEnd(&backstrm);
    free(comp_buf);
    free(out_buf);
    free(window);
    (void)comp_ret;
    (void)def_init_ret;
    (void)inf_init_ret;
    (void)inf_back_init_ret;
    (void)def_set_hdr_ret;
    (void)inf_ret;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)inf_back_end_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}