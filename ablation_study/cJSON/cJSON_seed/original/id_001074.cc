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
//<ID> 1074
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"values\":[1.1,2.2,3.3],\"flag\":0}";
    size_t json_len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, json_len);

    // step 2: Configure
    double new_values[3] = {4.4, 5.5, 6.6};
    cJSON *new_array = cJSON_CreateDoubleArray(new_values, 3);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "values", new_array);
    cJSON_AddTrueToObject(root, "processed");
    cJSON *count_item = cJSON_CreateNumber(3.0);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "flag", count_item);

    // step 3: Operate
    char *pretty = cJSON_Print(root);
    int buf_len = 256;
    char *prealloc = (char *)cJSON_malloc((size_t)buf_len);
    memset(prealloc, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, prealloc, buf_len, 1);
    cJSON *vals = cJSON_GetObjectItem(root, "values");
    cJSON *first_val_item = cJSON_GetArrayItem(vals, 0);
    double first_val = cJSON_GetNumberValue(first_val_item);
    cJSON *first_val_copy = cJSON_CreateNumber(first_val);
    cJSON_AddItemToObject(root, "first_value_copy", first_val_copy);

    // step 4: Validate & Cleanup
    cJSON_free(pretty);
    cJSON_free(prealloc);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}