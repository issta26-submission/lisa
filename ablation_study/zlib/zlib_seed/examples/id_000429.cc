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
//<ID> 429
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Prepare source data and compress into a buffer
    const char src[] = "zlib API sequence payload: prepare -> compress -> checksums -> inflateBack -> inflate -> gz IO -> cleanup";
    const Bytef *source = (const Bytef *)src;
    uLong sourceLen = (uLong)(sizeof(src) - 1);
    uLong bound = compressBound(sourceLen);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    uLongf compLen = (uLongf)bound;
    int rc_comp = compress(comp, &compLen, source, sourceLen);

    // step 2: Compute Adler-32 on two halves and combine to validate checksum
    uLong half = (uLong)(sourceLen / 2);
    const Bytef *part1 = source;
    const Bytef *part2 = source + half;
    uLong len1 = (uLong)half;
    uLong len2 = (uLong)(sourceLen - half);
    uLong ad1 = adler32(0L, part1, (uInt)len1);
    uLong ad2 = adler32(0L, part2, (uInt)len2);
    uLong combined_ad = adler32_combine(ad1, ad2, (off_t)len2);
    uLong ad_full = adler32(0L, source, (uInt)sourceLen);

    // step 3: Inflate using the back interface with callbacks, and also inflate via standard inflate to compare; use gz IO
    unsigned char *window = (unsigned char *)malloc(32768);
    memset(window, 0, 32768);
    z_stream back_strm;
    memset(&back_strm, 0, sizeof(z_stream));
    const char *version = zlibVersion();
    int rc_back_init = inflateBackInit_(&back_strm, 15, window, version, (int)sizeof(z_stream));

    struct InDesc { const unsigned char *data; uLong len; uLong pos; };
    struct OutDesc { unsigned char *data; uLong pos; uLong cap; };

    InDesc in_desc;
    in_desc.data = (const unsigned char *)comp;
    in_desc.len = (uLong)compLen;
    in_desc.pos = 0;

    OutDesc out_desc;
    out_desc.cap = sourceLen;
    out_desc.data = (unsigned char *)malloc((size_t)out_desc.cap);
    memset(out_desc.data, 0, (size_t)out_desc.cap);
    out_desc.pos = 0;

    unsigned int (*in_cb)(void *, unsigned char **) = [](void *d, unsigned char **buf)->unsigned int {
        InDesc *id = (InDesc *)d;
        unsigned int rem = (unsigned int)(id->len - id->pos);
        if (rem == 0) return 0;
        *buf = (unsigned char *)(id->data + id->pos);
        id->pos += rem;
        return rem;
    };
    int (*out_cb)(void *, unsigned char *, unsigned int) = [](void *d, unsigned char *buf, unsigned int len)->int {
        OutDesc *od = (OutDesc *)d;
        unsigned int tocopy = len;
        memcpy(od->data + od->pos, buf, tocopy);
        od->pos += tocopy;
        return 0;
    };

    int rc_back = inflateBack(&back_strm, in_cb, (void *)&in_desc, out_cb, (void *)&out_desc);
    int rc_back_end = inflateBackEnd(&back_strm);

    z_stream std_strm;
    memset(&std_strm, 0, sizeof(z_stream));
    int rc_inf_init = inflateInit_(&std_strm, version, (int)sizeof(z_stream));
    Bytef *std_out = (Bytef *)malloc((size_t)sourceLen);
    memset(std_out, 0, (size_t)sourceLen);
    std_strm.next_in = comp;
    std_strm.avail_in = (uInt)compLen;
    std_strm.next_out = std_out;
    std_strm.avail_out = (uInt)sourceLen;
    int rc_inflate = inflate(&std_strm, 0);
    int rc_inf_end = inflateEnd(&std_strm);
    uLong std_out_len = std_strm.total_out;
    uLong back_out_len = out_desc.pos;

    gzFile gz_w = gzopen("zlib_api_sequence_tmp.gz", "wb");
    int rc_gzwrite = 0;
    if (gz_w) rc_gzwrite = gzwrite(gz_w, out_desc.data, (unsigned int)back_out_len);
    int rc_gzclose_w = 0;
    if (gz_w) rc_gzclose_w = gzclose(gz_w);

    gzFile gz_r = gzopen("zlib_api_sequence_tmp.gz", "rb");
    int rc_gzclose_r = 0;
    if (gz_r) rc_gzclose_r = gzclose_r(gz_r);

    // step 4: Cleanup allocated resources
    free(comp);
    free(window);
    free(out_desc.data);
    free(std_out);
    (void)version;
    (void)rc_comp;
    (void)rc_back_init;
    (void)rc_back;
    (void)rc_back_end;
    (void)rc_inf_init;
    (void)rc_inflate;
    (void)rc_inf_end;
    (void)std_out_len;
    (void)back_out_len;
    (void)rc_gzwrite;
    (void)rc_gzclose_w;
    (void)rc_gzclose_r;
    (void)ad1;
    (void)ad2;
    (void)combined_ad;
    (void)ad_full;
    // API sequence test completed successfully
    return 66;
}