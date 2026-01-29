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
//<ID> 1237
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double values[4] = {1.5, 2.5, 3.5, 4.5};
    cJSON *double_array = cJSON_CreateDoubleArray(values, 4);
    cJSON *extra_array = cJSON_CreateArray();
    cJSON *root = cJSON_CreateObject();
    cJSON *maybe_null = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddItemToObject(root, "double_values", double_array);
    cJSON_AddItemToObject(root, "extras", extra_array);
    cJSON_AddItemToObject(root, "maybe_null", maybe_null);
    cJSON_bool null_flag = cJSON_IsNull(maybe_null);
    cJSON *null_flag_item = cJSON_CreateBool(null_flag);
    cJSON_AddItemToObject(root, "is_null_flag", null_flag_item);
    cJSON *first_elem = cJSON_GetArrayItem(double_array, 0);
    double first_val = cJSON_GetNumberValue(first_elem);
    cJSON *computed = cJSON_CreateNumber(first_val + 10.0);
    cJSON_AddItemToArray(extra_array, computed);

    // step 3: Operate & Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}