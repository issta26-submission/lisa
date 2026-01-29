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
//<ID> 486
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
    cJSON_AddItemToObject(root, "arr", arr);

    // step 2: Configure
    cJSON *num0 = cJSON_CreateNumber(1.0);
    cJSON *num1 = cJSON_CreateNumber(2.5);
    cJSON *num2 = cJSON_CreateNumber(4.0);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON *greeting = cJSON_CreateString("hello");
    cJSON_AddItemToObject(root, "greeting", greeting);

    // step 3: Operate
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    double second_value = cJSON_GetNumberValue(second_item);
    cJSON *computed = cJSON_CreateNumber(second_value * 3.0);
    cJSON_AddItemToObject(root, "computed", computed);

    // step 4: Validate and Cleanup
    cJSON *computed_item = cJSON_GetObjectItem(root, "computed");
    double computed_value = cJSON_GetNumberValue(computed_item);
    (void)computed_value;
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}