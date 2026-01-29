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
//<ID> 364
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *const colors[3] = { "red", "green", "blue" };
    cJSON *root = cJSON_CreateObject();
    cJSON *colors_array = cJSON_CreateStringArray(colors, 3);
    cJSON *name = cJSON_CreateString("palette");
    cJSON_AddItemToObject(root, "colors", colors_array);
    cJSON_AddItemToObject(root, "name", name);

    // step 2: Configure
    char *print_buffer = (char *)cJSON_malloc(512);
    memset(print_buffer, 0, 512);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, print_buffer, (int)512, 1);
    cJSON *description = cJSON_CreateString("A simple color palette");
    cJSON_AddItemToObject(root, "description", description);

    // step 3: Operate & Validate
    cJSON *missing = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "missing", missing);
    cJSON_bool was_null = cJSON_IsNull(missing);
    cJSON_bool root_is_obj = cJSON_IsObject(root);
    cJSON *retrieved_name = cJSON_GetObjectItem(root, "name");
    char *name_val = cJSON_GetStringValue(retrieved_name);
    (void)was_null;
    (void)root_is_obj;
    (void)name_val;
    (void)printed_ok;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(print_buffer);
    // API sequence test completed successfully
    return 66;
}