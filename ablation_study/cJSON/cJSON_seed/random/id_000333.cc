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
//<ID> 333
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
    cJSON *true_item = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON *second_elem = (cJSON *)0;
    cJSON *dup = (cJSON *)0;
    cJSON *got_array = (cJSON *)0;
    cJSON_bool first_is_string = (cJSON_bool)0;
    cJSON_bool second_is_object = (cJSON_bool)0;
    cJSON_bool dup_array_is_object = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure
    str_item = cJSON_CreateString("sample_text");
    cJSON_AddItemToArray(arr, str_item);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, true_item);

    // step 4: Operate
    first_elem = cJSON_GetArrayItem(arr, 0);
    second_elem = cJSON_GetArrayItem(arr, 1);
    first_is_string = cJSON_IsString(first_elem);
    second_is_object = cJSON_IsObject(second_elem);
    dup = cJSON_Duplicate(root, (cJSON_bool)1);

    // step 5: Validate
    got_array = cJSON_GetObjectItem(dup, "items");
    dup_array_is_object = cJSON_IsObject(got_array);

    // step 6: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}