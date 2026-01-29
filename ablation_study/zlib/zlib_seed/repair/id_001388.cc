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
//<ID> 1388
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize
    const Bytef src[] = "Zlib API sequence test: deflate -> write gz -> read gz -> inflate with dictionary and reset deflate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    int dinit = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)src;
    dstrm.avail_in = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);

    // step 2: Configure
    uLong bound = deflateBound(&dstrm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    int dres = deflate(&dstrm, 4);
    uLong comp_len = dstrm.total_out;
    int dreset = deflateReset(&dstrm);

    // step 3: Operate
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gwrote = gzwrite(gw, comp_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int gwc = gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *read_buf = (Bytef *)malloc((size_t)(comp_len + 1));
    memset(read_buf, 0, (int)(comp_len + 1));
    int gread = gzread(gr, read_buf, (unsigned int)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)comp_len));
    int grc = gzclose_r(gr);

    // step 4: Validate & Cleanup
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int iinit = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    istrm.next_in = read_buf;
    istrm.avail_in = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len);
    Bytef *out_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(out_buf, 0, (int)(src_len + 16));
    istrm.next_out = out_buf;
    istrm.avail_out = (uInt)((src_len + 16) > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)(src_len + 16));
    int iret = inflate(&istrm, 0);
    int iset = inflateSetDictionary(&istrm, src, (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len));
    int iend = inflateEnd(&istrm);
    int dend = deflateEnd(&dstrm);
    (void)dinit;
    (void)dres;
    (void)comp_len;
    (void)dreset;
    (void)gwrote;
    (void)gwc;
    (void)gread;
    (void)grc;
    (void)iinit;
    (void)iret;
    (void)iset;
    (void)iend;
    (void)dend;
    (void)version;
    free(comp_buf);
    free(read_buf);
    free(out_buf);
    // API sequence test completed successfully
    return 66;
}