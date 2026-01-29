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
//<ID> 1232
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double values[3];
    values[0] = 1.1;
    values[1] = 2.2;
    values[2] = 3.3;
    cJSON *root = cJSON_CreateObject();
    cJSON *double_array = cJSON_CreateDoubleArray(values, 3);
    cJSON_AddItemToObject(root, "doubles", double_array);

    // step 2: Configure
    cJSON *mixed = cJSON_CreateArray();
    cJSON_AddItemToArray(mixed, cJSON_CreateNumber(42.0));
    cJSON_AddItemToArray(mixed, cJSON_CreateString("example"));
    cJSON_AddItemToObject(root, "mixed", mixed);
    cJSON *empty_arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "empty", empty_arr);

    // step 3: Operate & Validate
    cJSON *maybe_null = cJSON_CreateNull();
    cJSON_bool was_null = cJSON_IsNull(maybe_null);
    cJSON *null_flag = cJSON_CreateBool(was_null);
    cJSON_AddItemToObject(root, "was_null", null_flag);
    cJSON *first_item = cJSON_GetArrayItem(double_array, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON_AddItemToObject(root, "first_times_two", cJSON_CreateNumber(first_val * 2.0));
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(maybe_null);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}