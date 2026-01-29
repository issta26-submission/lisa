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
//<ID> 623
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
    cJSON *num_item = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    cJSON *false_item = (cJSON *)0;
    cJSON *retrieved_item = (cJSON *)0;
    cJSON_bool is_false = 0;
    double first_num = 0.0;
    const char *second_str = (const char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num_item = cJSON_CreateNumber(3.14);
    str_item = cJSON_CreateString("hello");
    false_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToArray(arr, num_item);
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "payload", arr);
    cJSON_AddItemToObject(root, "status", false_item);

    // step 4: Operate
    retrieved_item = cJSON_GetArrayItem(arr, 1);
    second_str = cJSON_GetStringValue(retrieved_item);
    is_false = cJSON_IsFalse(false_item);

    // step 5: Validate
    first_num = cJSON_GetNumberValue(cJSON_GetArrayItem(arr, 0));
    (void)second_str;
    (void)is_false;
    (void)first_num;
    (void)retrieved_item;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}