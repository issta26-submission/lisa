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
//<ID> 1121
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
    cJSON *value_item = cJSON_CreateNumber(3.1415);
    cJSON_AddItemToObject(root, "value", value_item);
    const double dbls[] = {1.1, 2.2, 3.3};
    cJSON *double_arr = cJSON_CreateDoubleArray(dbls, 3);
    cJSON_AddItemToObject(root, "doubles", double_arr);

    // step 2: Configure
    char *old_name = cJSON_SetValuestring(name_item, "updated_name");
    cJSON_AddItemToObject(root, "was_name_set", cJSON_CreateString(old_name ? "yes" : "no"));

    // step 3: Operate
    cJSON_bool is_val_number = cJSON_IsNumber(value_item);
    cJSON_AddItemToObject(root, "value_is_number", cJSON_CreateNumber((double)is_val_number));
    int arr_size = cJSON_GetArraySize(double_arr);
    cJSON *first_elem = cJSON_GetArrayItem(double_arr, 0);
    double first_val = cJSON_GetNumberValue(first_elem);
    double computed = first_val + (double)arr_size;
    cJSON_AddItemToObject(root, "computed", cJSON_CreateNumber(computed));

    // step 4: Validate & Cleanup
    char *out = cJSON_PrintUnformatted(root);
    char *prealloc = (char *)cJSON_malloc(256);
    memset(prealloc, 0, 256);
    cJSON_PrintPreallocated(root, prealloc, 256, 1);
    cJSON_free(out);
    cJSON_free(prealloc);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}