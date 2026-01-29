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
//<ID> 1107
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"nums\":[1.5,2.5],\"raw\":\"raw_data\",\"name\":\"old_name\"}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 2: Configure
    double extra_vals[3] = {3.14, 2.71, 1.41};
    cJSON *extra_array = cJSON_CreateDoubleArray(extra_vals, 3);
    cJSON_AddItemToObject(root, "dbls", extra_array);
    cJSON_AddTrueToObject(root, "active");
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON_SetValuestring(name_item, "new_name");

    // step 3: Operate
    cJSON *raw_item = cJSON_GetObjectItem(root, "raw");
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw_item);
    cJSON *nums_item = cJSON_GetObjectItem(root, "nums");
    cJSON *first_num = cJSON_GetArrayItem(nums_item, 0);
    cJSON_bool first_is_number = cJSON_IsNumber(first_num);
    double first_val = cJSON_GetNumberValue(first_num);
    double combined = first_val + (double)raw_is_raw + (double)first_is_number;
    cJSON_AddNumberToObject(root, "computed", combined);

    // step 4: Validate & Cleanup
    char *unfmt = cJSON_PrintUnformatted(root);
    char *prealloc = (char *)cJSON_malloc(512);
    memset(prealloc, 0, 512);
    cJSON_PrintPreallocated(root, prealloc, 512, 1);
    cJSON_free(unfmt);
    cJSON_free(prealloc);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}