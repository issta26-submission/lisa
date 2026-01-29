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
//<ID> 1102
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"values\":[1.5,2.5],\"raw\":\"<rawtext>\",\"label\":\"old_label\",\"num\":42.0}";
    size_t json_len = sizeof(json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json, json_len, &parse_end, 1);

    // step 2: Configure
    double extra_vals[3] = {3.3, 4.4, 5.5};
    cJSON *double_array = cJSON_CreateDoubleArray(extra_vals, 3);
    cJSON_AddItemToObject(root, "doubles", double_array);
    cJSON *active_flag = cJSON_AddTrueToObject(root, "active");
    cJSON *label_item = cJSON_GetObjectItem(root, "label");
    char *updated_label = cJSON_SetValuestring(label_item, "new_label");

    // step 3: Operate
    cJSON *raw_item = cJSON_GetObjectItem(root, "raw");
    cJSON_bool raw_check = cJSON_IsRaw(raw_item);
    cJSON *num_item = cJSON_GetObjectItem(root, "num");
    cJSON_bool num_check = cJSON_IsNumber(num_item);
    cJSON *values_item = cJSON_GetObjectItem(root, "values");
    int values_count = cJSON_GetArraySize(values_item);
    char *pretty = cJSON_Print(root);
    char *compact = cJSON_PrintUnformatted(root);
    char *prealloc = (char *)cJSON_malloc(512);
    memset(prealloc, 0, 512);
    cJSON_PrintPreallocated(root, prealloc, 512, 1);

    // step 4: Validate & Cleanup
    cJSON_free(prealloc);
    cJSON_free(pretty);
    cJSON_free(compact);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}