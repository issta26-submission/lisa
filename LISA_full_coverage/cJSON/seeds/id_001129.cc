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
//<ID> 1129
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("initial_name");
    cJSON_AddItemToObject(root, "name", name_item);
    double vals[] = {1.5, 2.5, 3.75};
    cJSON *double_arr = cJSON_CreateDoubleArray(vals, 3);
    cJSON_AddItemToObject(root, "double_list", double_arr);
    cJSON *value_item = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(root, "value", value_item);

    // step 2: Configure
    cJSON *name_ptr = cJSON_GetObjectItem(root, "name");
    char *old_name = cJSON_SetValuestring(name_ptr, "updated_name");
    cJSON_AddItemToObject(root, "original_name", cJSON_CreateString(old_name ? old_name : "none"));

    // step 3: Operate
    cJSON *val_ptr = cJSON_GetObjectItem(root, "value");
    cJSON_bool is_num = cJSON_IsNumber(val_ptr);
    double base = cJSON_GetNumberValue(val_ptr);
    cJSON *first_double = cJSON_GetArrayItem(double_arr, 0);
    double first_val = cJSON_IsNumber(first_double) ? cJSON_GetNumberValue(first_double) : 0.0;
    double computed = base + (double)is_num + first_val;
    cJSON_AddNumberToObject(root, "computed", computed);

    // step 4: Validate & Cleanup
    char *flat = cJSON_PrintUnformatted(root);
    char *buf = (char *)cJSON_malloc(256);
    memset(buf, 0, 256);
    cJSON_PrintPreallocated(root, buf, 256, 1);
    cJSON_free(flat);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}