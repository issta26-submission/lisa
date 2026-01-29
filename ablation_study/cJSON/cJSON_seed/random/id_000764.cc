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
//<ID> 764
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
    cJSON *nested = (cJSON *)0;
    cJSON *str0 = (cJSON *)0;
    cJSON *str1 = (cJSON *)0;
    cJSON *retrieved_item0 = (cJSON *)0;
    cJSON *retrieved_item1 = (cJSON *)0;
    char *retrieved_str0 = (char *)0;
    char *retrieved_str1 = (char *)0;
    cJSON_bool root_is_object = 0;
    int validation_score = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    nested = cJSON_CreateObject();
    str0 = cJSON_CreateString("hello");
    str1 = cJSON_CreateString("world");

    // step 3: Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "meta", nested);
    cJSON_AddItemToArray(arr, str0);
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToObject(nested, "description", cJSON_CreateString("container"));

    // step 4: Operate
    retrieved_item0 = cJSON_GetArrayItem(arr, 0);
    retrieved_item1 = cJSON_GetArrayItem(arr, 1);
    retrieved_str0 = cJSON_GetStringValue(retrieved_item0);
    retrieved_str1 = cJSON_GetStringValue(retrieved_item1);
    root_is_object = cJSON_IsObject(root);

    // step 5: Validate
    validation_score = (retrieved_item0 != (cJSON *)0) + (retrieved_item1 != (cJSON *)0) + (retrieved_str0 != (char *)0) + (retrieved_str1 != (char *)0) + (root_is_object != 0);
    (void)validation_score;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}