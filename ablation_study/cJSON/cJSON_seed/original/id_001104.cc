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
//<ID> 1104
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"value\":42,\"raw\":\"<raw_content>\",\"name\":\"old_name\"}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 2: Configure
    double nums[] = {1.5, 2.5, 3.5};
    cJSON *dbl_array = cJSON_CreateDoubleArray(nums, 3);
    cJSON_AddItemToObject(root, "doubles", dbl_array);
    cJSON_AddTrueToObject(root, "enabled");
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *replaced = cJSON_SetValuestring(name_item, "new_name");

    // step 3: Operate
    cJSON *val_item = cJSON_GetObjectItem(root, "value");
    cJSON_bool val_is_number = cJSON_IsNumber(val_item);
    cJSON *raw_item = cJSON_GetObjectItem(root, "raw");
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw_item);
    char *unformatted = cJSON_PrintUnformatted(root);
    char *prealloc = (char *)cJSON_malloc(256);
    memset(prealloc, 0, 256);
    cJSON_PrintPreallocated(root, prealloc, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_free(replaced);
    cJSON_free(unformatted);
    cJSON_free(prealloc);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}