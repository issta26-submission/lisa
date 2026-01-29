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
//<ID> 1239
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    double numbers[3];
    numbers[0] = 1.5;
    numbers[1] = 2.5;
    numbers[2] = 3.5;
    cJSON *double_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *empty_array = cJSON_CreateArray();
    cJSON *null_item = cJSON_CreateNull();
    cJSON_bool null_check = cJSON_IsNull(null_item);
    cJSON *null_flag = cJSON_CreateBool(null_check);

    // step 3: Operate & Validate
    cJSON_AddItemToObject(root, "doubles", double_array);
    cJSON_AddItemToObject(root, "empty", empty_array);
    cJSON_AddItemToObject(root, "nullable", null_item);
    cJSON_AddItemToObject(root, "null_was_null", null_flag);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}