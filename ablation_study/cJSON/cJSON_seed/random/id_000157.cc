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
//<ID> 157
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
    cJSON *str1 = (cJSON *)0;
    cJSON *str2 = (cJSON *)0;
    cJSON *ttrue = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *item0 = (cJSON *)0;
    int size_before = 0;
    int size_after = 0;
    cJSON_bool added_a = (cJSON_bool)0;
    cJSON_bool added_b = (cJSON_bool)0;
    cJSON_bool added_c = (cJSON_bool)0;
    cJSON_bool first_was_string = (cJSON_bool)0;
    char *json_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str1 = cJSON_CreateString("first");
    ttrue = cJSON_CreateTrue();
    str2 = cJSON_CreateString("third");

    // step 3: Configure
    added_a = cJSON_AddItemToArray(arr, str1);
    added_b = cJSON_AddItemToArray(arr, ttrue);
    added_c = cJSON_AddItemToArray(arr, str2);
    cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    size_before = cJSON_GetArraySize(arr);
    item0 = cJSON_GetArrayItem(arr, 0);
    first_was_string = cJSON_IsString(item0);
    detached = cJSON_DetachItemFromArray(arr, 1);
    size_after = cJSON_GetArraySize(arr);

    // step 5: Validate
    cJSON_AddNumberToObject(root, "size_before", (double)size_before);
    cJSON_AddNumberToObject(root, "size_after", (double)size_after);
    cJSON_AddNumberToObject(root, "first_was_string", (double)first_was_string);
    cJSON_AddNumberToObject(root, "add_result_a", (double)added_a);
    cJSON_AddNumberToObject(root, "add_result_b", (double)added_b);
    cJSON_AddNumberToObject(root, "add_result_c", (double)added_c);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}