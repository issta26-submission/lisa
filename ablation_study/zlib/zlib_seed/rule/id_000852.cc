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
//<ID> 852
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_zlib_api_sequence() {
    // step 1: Declarations & prepare buffers
    const char *ver = zlibVersion();
    const uLong INPUT_LEN = 256UL;
    Bytef *input = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(input, 'A', (size_t)INPUT_LEN);
    z_stream dstrm;
    memset(&dstrm, 0, sizeof(dstrm));
    dstrm.zalloc = NULL;
    dstrm.zfree = NULL;
    dstrm.opaque = NULL;
    dstrm.next_in = input;
    dstrm.avail_in = (uInt)INPUT_LEN;

    // step 2: Setup - initialize deflate and prepare output buffer
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, INPUT_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;

    // step 3: Core operations - compress, initialize inflate with window bits, decompress, validate, use gzread
    int def_status = deflate(&dstrm, 4); /* Z_FINISH */
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    int windowBits = 15;
    inflateInit2_(&istrm, windowBits, ver, (int)sizeof(z_stream));
    istrm.next_in = comp;
    istrm.avail_in = (uInt)dstrm.total_out;
    Bytef *out = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(out, 0, (size_t)INPUT_LEN);
    istrm.next_out = out;
    istrm.avail_out = (uInt)INPUT_LEN;
    int inf_status = inflate(&istrm, 4); /* Z_FINISH */
    uLong out_adler = adler32(0UL, out, (uInt)INPUT_LEN);

    gzFile gw = gzopen("test_zlib_api_seq.gz", "wb");
    gzwrite(gw, comp, (unsigned int)dstrm.total_out);
    gzclose(gw);
    gzFile gr = gzopen("test_zlib_api_seq.gz", "rb");
    Bytef *readbuf = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(readbuf, 0, (size_t)INPUT_LEN);
    int read_bytes = gzread(gr, readbuf, (unsigned int)INPUT_LEN);

    // step 4: Cleanup - finalize streams and free resources
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    gzclose(gr);
    free(input);
    free(comp);
    free(out);
    free(readbuf);
    (void)ver;
    (void)bound;
    (void)def_status;
    (void)inf_status;
    (void)out_adler;
    (void)read_bytes;
    // API sequence test completed successfully
    return 66;
}