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
//<ID> 1384
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Initialize - prepare data, deflate compressed buffer
    const Bytef src[] = "Zlib API sequence: prepare compressed data, write with gz, read back dictionary, set inflate dictionary and inflate";
    uLong src_len = (uLong)(sizeof(src) - 1);
    const char *version = zlibVersion();
    z_stream dstrm;
    memset(&dstrm, 0, (int)sizeof(dstrm));
    int dinit = deflateInit_(&dstrm, 6, version, (int)sizeof(z_stream));
    dstrm.next_in = (Bytef *)src;
    dstrm.avail_in = (uInt)src_len;
    uLong bound = deflateBound(&dstrm, src_len);
    Bytef *comp_buf = (Bytef *)malloc((size_t)bound);
    memset(comp_buf, 0, (int)bound);
    dstrm.next_out = comp_buf;
    dstrm.avail_out = (uInt)(bound > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)bound);
    int ddef = deflate(&dstrm, 4);
    uLong comp_len = dstrm.total_out;
    int dreset = deflateReset(&dstrm);

    // step 2: Configure - write original data via gzwrite and read it back to use as inflate dictionary
    gzFile gw = gzopen("test_zlib_api_sequence.gz", "wb");
    int gwr = gzwrite(gw, src, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int gwc = gzclose(gw);
    gzFile grf = gzopen("test_zlib_api_sequence.gz", "rb");
    Bytef *dict_buf = (Bytef *)malloc((size_t)(src_len + 1));
    memset(dict_buf, 0, (int)(src_len + 1));
    int gr = gzread(grf, dict_buf, (unsigned int)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (unsigned int)src_len));
    int grc = gzclose_r(grf);

    // step 3: Operate - initialize inflate, set dictionary from read data, and inflate compressed buffer
    z_stream istrm;
    memset(&istrm, 0, (int)sizeof(istrm));
    int iinit = inflateInit_(&istrm, version, (int)sizeof(z_stream));
    uInt dict_len = (uInt)(src_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)src_len);
    int iset = inflateSetDictionary(&istrm, dict_buf, dict_len);
    istrm.next_in = comp_buf;
    istrm.avail_in = (uInt)(comp_len > 0xFFFFFFFFUL ? 0xFFFFFFFFU : (uInt)comp_len);
    Bytef *decomp_buf = (Bytef *)malloc((size_t)(src_len + 16));
    memset(decomp_buf, 0, (int)(src_len + 16));
    istrm.next_out = decomp_buf;
    istrm.avail_out = (uInt)(src_len + 16);
    int iret = inflate(&istrm, 0);
    uLong decomp_written = istrm.total_out;

    // step 4: Validate & Cleanup
    int dend = deflateEnd(&dstrm);
    int iend = inflateEnd(&istrm);
    (void)dinit;
    (void)ddef;
    (void)comp_len;
    (void)dreset;
    (void)gwr;
    (void)gwc;
    (void)gr;
    (void)grc;
    (void)iinit;
    (void)iset;
    (void)iret;
    (void)decomp_written;
    (void)dend;
    (void)iend;
    (void)version;
    free(comp_buf);
    free(dict_buf);
    free(decomp_buf);
    // API sequence test completed successfully
    return 66;
}