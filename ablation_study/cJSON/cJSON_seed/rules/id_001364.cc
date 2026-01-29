#include <cJSON.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 1364
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *raw_item = NULL;
    char *print_buf = NULL;
    void *alloc_mem = NULL;
    int buf_len = 256;
    cJSON_bool replaced = 0;
    cJSON_bool is_raw = 0;
    cJSON_bool printed_ok = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "field", "initial_value");
    raw_item = cJSON_CreateRaw("{\"inner\":123}");
    alloc_mem = cJSON_malloc((size_t)buf_len);
    memset(alloc_mem, 0, (size_t)buf_len);
    print_buf = (char *)alloc_mem;

    // step 3: Operate / Validate
    replaced = cJSON_ReplaceItemInObject(root, "field", raw_item);
    is_raw = cJSON_IsRaw(raw_item);
    printed_ok = cJSON_PrintPreallocated(root, print_buf, buf_len, is_raw);
    (void)replaced;
    (void)printed_ok;

    // step 4: Cleanup
    cJSON_free(print_buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}