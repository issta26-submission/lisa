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
//<ID> 1125
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("old_name");
    cJSON_AddItemToObject(root, "name", name_item);
    cJSON *value_item = cJSON_CreateNumber(7.7);
    cJSON_AddItemToObject(root, "value", value_item);
    const double arr_vals[] = {1.1, 2.2, 3.3};
    cJSON *double_arr = cJSON_CreateDoubleArray(arr_vals, 3);
    cJSON_AddItemToObject(root, "dbls", double_arr);

    // step 2: Configure
    char *old_name_ptr = cJSON_SetValuestring(name_item, "new_name"); // owned by cJSON; do not free separately
    cJSON *meta_obj = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta_obj);
    cJSON *marker = cJSON_CreateString("configured");
    cJSON_AddItemToObject(meta_obj, "status", marker);

    // step 3: Operate
    cJSON_bool is_value_number = cJSON_IsNumber(value_item);
    double value_num = cJSON_GetNumberValue(value_item);
    double computed = value_num + arr_vals[0] + (double)is_value_number;
    cJSON_AddNumberToObject(root, "computed", computed);

    // step 4: Validate & Cleanup
    char *unformatted = cJSON_PrintUnformatted(root);
    char *prealloc = (char *)cJSON_malloc(512);
    memset(prealloc, 0, 512);
    cJSON_PrintPreallocated(root, prealloc, 512, 1);
    cJSON_free(unformatted);
    cJSON_free(prealloc);
    // do NOT free old_name_ptr: it is managed by the cJSON object (avoids double free)
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}