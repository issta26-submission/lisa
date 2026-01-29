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
//<ID> 487
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

    // step 2: Configure
    cJSON *num0 = cJSON_CreateNumber(3.14);
    cJSON *str1 = cJSON_CreateString("example");
    cJSON *num2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "my_array", arr);
    cJSON *title = cJSON_CreateString("root_title");
    cJSON_AddItemToObject(root, "title", title);

    // step 3: Operate
    cJSON *item = cJSON_GetArrayItem(arr, 2);
    double extracted = cJSON_GetNumberValue(item);
    cJSON *result = cJSON_CreateNumber(extracted + 0.5);
    cJSON_AddItemToObject(root, "result", result);

    // step 4: Validate and Cleanup
    cJSON *res_check = cJSON_GetObjectItem(root, "result");
    double validated = cJSON_GetNumberValue(res_check);
    (void)validated;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}