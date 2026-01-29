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
//<ID> 1456
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source and compressed buffers)
    const Bytef src[] = "zlib API sequence: prepare data, compress, init deflate header, init inflate (normal and back)";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(src_len);
    Bytef * compBuf = (Bytef *)malloc((size_t)bound);
    uLongf compLen = (uLongf)bound;
    (void)compress2(compBuf, &compLen, src, src_len, 6);

    // step 2: Configure (initialize deflate stream and set gzip header; initialize inflate streams)
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    gz_header hdr;
    memset(&hdr, 0, (int)sizeof(hdr));
    hdr.text = 0;
    hdr.name = (Bytef *)"test_zlib_api_sequence";
    hdr.name_max = (uInt)strlen((const char *)hdr.name);
    int set_hdr_ret = deflateSetHeader(&dstrm, &hdr);
    int def_end_ret = deflateEnd(&dstrm);

    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    z_stream bstrm;
    memset(&bstrm, 0, (int)sizeof(bstrm));
    unsigned char * window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    int inf_back_init_ret = inflateBackInit_(&bstrm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Operate (decompress the previously compressed buffer with inflate)
    Bytef * outBuf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(outBuf, 0, (int)(src_len + 16));
    istrm.next_in = compBuf;
    istrm.avail_in = (uInt)((compLen > 0xFFFFFFFFUL) ? 0xFFFFFFFFUL : (uLong)compLen);
    istrm.next_out = outBuf;
    istrm.avail_out = (uInt)((src_len + 16 > 0xFFFFFFFFUL) ? 0xFFFFFFFFUL : (uLong)(src_len + 16));
    int inf_ret = inflate(&istrm, 0);

    // step 4: Validate & Cleanup (end inflate streams, free buffers)
    int inf_end_ret = inflateEnd(&istrm);
    int inf_back_end_ret = inflateBackEnd(&bstrm);
    free(window);
    free(compBuf);
    free(outBuf);

    (void)def_init_ret;
    (void)set_hdr_ret;
    (void)def_end_ret;
    (void)inf_init_ret;
    (void)inf_back_init_ret;
    (void)inf_ret;
    (void)inf_end_ret;
    (void)inf_back_end_ret;
    (void)version;

    // API sequence test completed successfully
    return 66;
}