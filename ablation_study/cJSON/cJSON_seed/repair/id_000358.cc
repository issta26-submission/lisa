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
//<ID> 358
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num0 = cJSON_CreateNumber(42.0);
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *flag = cJSON_CreateFalse();

    // step 2: Configure
    cJSON_AddItemToObject(root, "array", arr);
    cJSON_AddItemToObject(root, "flag", flag);
    cJSON_AddNumberToObject(root, "root_value", 7.0);
    cJSON_InsertItemInArray(arr, 0, num0);
    cJSON_InsertItemInArray(arr, 1, num1);

    // step 3: Operate and Validate
    char json_buf[] = " { \"a\": 1, \"b\": false } ";
    cJSON_Minify(json_buf);
    cJSON_bool flag_is_false = cJSON_IsFalse(flag);
    cJSON_AddNumberToObject(root, "flag_is_false", (double)flag_is_false);
    cJSON *first = cJSON_GetArrayItem(arr, 0);
    double v = cJSON_GetNumberValue(first);
    cJSON_AddNumberToObject(root, "double_first", v * 2.0);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}