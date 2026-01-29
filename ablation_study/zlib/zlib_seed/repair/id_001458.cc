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
//<ID> 1458
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize (prepare source and compress)
    const Bytef src[] = "Zlib sequence: compress and inflate using deflateInit_, deflateSetHeader, inflateBackInit_, inflateInit_, inflate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(src_len);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    uLongf comp_len = (uLongf)bound;
    int comp_ret = compress2(comp, &comp_len, src, src_len, 6);

    // step 2: Configure (init deflate + set header, init inflate and inflateBack)
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    const char *version = zlibVersion();
    int def_init_ret = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    gz_header head;
    memset(&head, 0, (int)sizeof(head));
    head.text = 1;
    head.time = 0x5F3759DF;
    int def_set_head_ret = deflateSetHeader(&dstrm, &head);

    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int inf_init_ret = inflateInit_(&istrm, version, (int)sizeof(z_stream));

    z_stream bstrm;
    memset(&bstrm, 0, (int)sizeof(bstrm));
    unsigned char window[32768];
    memset(window, 0, (int)sizeof(window));
    int inf_back_init_ret = inflateBackInit_(&bstrm, 15, window, version, (int)sizeof(z_stream));

    // step 3: Operate (inflate compressed data into output buffer)
    Bytef *out = (Bytef *)malloc((size_t)(src_len + 64));
    memset(out, 0, (int)(src_len + 64));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)((comp_len > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (uInt)comp_len);
    istrm.next_out = out;
    istrm.avail_out = (uInt)((src_len + 64 > 0xFFFFFFFFUL) ? 0xFFFFFFFFU : (uInt)(src_len + 64));
    int inf_ret = inflate(&istrm, 0);

    // step 4: Validate & Cleanup
    int def_end_ret = deflateEnd(&dstrm);
    int inf_end_ret = inflateEnd(&istrm);
    int inf_back_end_ret = inflateBackEnd(&bstrm);
    free(comp);
    free(out);
    (void)comp_ret;
    (void)def_init_ret;
    (void)def_set_head_ret;
    (void)inf_init_ret;
    (void)inf_back_init_ret;
    (void)inf_ret;
    (void)def_end_ret;
    (void)inf_end_ret;
    (void)inf_back_end_ret;
    (void)version;
    // API sequence test completed successfully
    return 66;
}