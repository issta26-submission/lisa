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
//<ID> 857
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

    // step 2: Initialize deflate, compress to memory and write compressed data to a .gz file
    deflateInit_(&dstrm, 6, ver, (int)sizeof(z_stream));
    uLong bound = deflateBound(&dstrm, INPUT_LEN);
    Bytef *comp = (Bytef *)malloc((size_t)bound);
    memset(comp, 0, (size_t)bound);
    dstrm.next_out = comp;
    dstrm.avail_out = (uInt)bound;
    int def_status = deflate(&dstrm, 4); /* Z_FINISH */
    gzFile gw = gzopen("test_zlib_api_seq.gz", "wb");
    gzwrite(gw, comp, (unsigned int)dstrm.total_out);
    gzclose(gw);

    // step 3: Read compressed data back, initialize inflate with windowBits, set adler and run inflate
    Bytef *comp_read = (Bytef *)malloc((size_t)bound);
    memset(comp_read, 0, (size_t)bound);
    gzFile gr = gzopen("test_zlib_api_seq.gz", "rb");
    int bytes_read = gzread(gr, comp_read, (unsigned int)bound);
    gzclose(gr);
    z_stream istrm;
    memset(&istrm, 0, sizeof(istrm));
    istrm.zalloc = NULL;
    istrm.zfree = NULL;
    istrm.opaque = NULL;
    int windowBits = 15;
    inflateInit2_(&istrm, windowBits, ver, (int)sizeof(z_stream));
    istrm.next_in = comp_read;
    istrm.avail_in = (uInt)bytes_read;
    Bytef *out = (Bytef *)malloc((size_t)INPUT_LEN);
    memset(out, 0, (size_t)INPUT_LEN);
    istrm.next_out = out;
    istrm.avail_out = (uInt)INPUT_LEN;
    uLong checksum = adler32(0UL, input, (uInt)INPUT_LEN);
    istrm.adler = checksum;
    int inf_status = inflate(&istrm, 0);

    // step 4: Cleanup all resources
    deflateEnd(&dstrm);
    inflateEnd(&istrm);
    free(input);
    free(comp);
    free(comp_read);
    free(out);
    (void)ver;
    (void)bound;
    (void)def_status;
    (void)inf_status;
    (void)checksum;
    // API sequence test completed successfully
    return 66;
}