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
//<ID> 321
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *arr = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *num_item = (cJSON *)0;
    double pi_val = 0.0;
    cJSON_bool arr_check = (cJSON_bool)0;

    // step 2: Initialize / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddNumberToObject(root, "pi", 3.1415);
    str_item = cJSON_CreateString("greeting_string");
    cJSON_AddItemToObject(root, "greeting", str_item);

    // step 3: Operate
    arr_check = cJSON_IsArray(arr);
    num_item = cJSON_GetObjectItem(root, "pi");
    pi_val = cJSON_GetNumberValue(num_item);
    detached = cJSON_DetachItemViaPointer(root, str_item);
    cJSON_AddItemToArray(arr, detached);

    // step 4: Validate / Modify
    cJSON_AddNumberToObject(root, "pi_copy", pi_val);

    // step 5: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}