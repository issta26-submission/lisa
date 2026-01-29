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
//<ID> 1365
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *initial = NULL;
    cJSON *replacement = NULL;
    cJSON *retrieved = NULL;
    char *prealloc_buf = NULL;
    cJSON_bool replace_result = 0;
    cJSON_bool is_raw_result = 0;
    cJSON_bool print_prealloc_ok = 0;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    initial = cJSON_CreateString("initial_value");
    cJSON_AddItemToObject(root, "field", initial);
    prealloc_buf = (char *)cJSON_malloc(256);
    memset(prealloc_buf, 0, 256);
    replacement = cJSON_CreateRaw("UNPARSED_RAW_FRAGMENT");
    replace_result = cJSON_ReplaceItemInObject(root, "field", replacement);

    // step 3: Operate / Validate
    retrieved = cJSON_GetObjectItem(root, "field");
    is_raw_result = cJSON_IsRaw(retrieved);
    print_prealloc_ok = cJSON_PrintPreallocated(root, prealloc_buf, 256, 0);

    // step 4: Cleanup
    cJSON_free(prealloc_buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}