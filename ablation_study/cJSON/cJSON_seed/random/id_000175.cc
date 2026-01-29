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
//<ID> 175
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
    cJSON *true_item = (cJSON *)0;
    cJSON *got0 = (cJSON *)0;
    cJSON *got1 = (cJSON *)0;
    int size_before = 0;
    int size_after = 0;
    cJSON_bool insert_ok = (cJSON_bool)0;
    cJSON_bool first_is_true = (cJSON_bool)0;
    cJSON_bool second_is_string = (cJSON_bool)0;
    char *json_text = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str1 = cJSON_CreateString("second");
    str2 = cJSON_CreateString("third");
    true_item = cJSON_CreateTrue();

    // step 3: Configure
    cJSON_AddItemToArray(arr, str1);
    cJSON_AddItemToArray(arr, str2);
    cJSON_AddItemToObject(root, "items", arr);

    // step 4: Operate
    size_before = cJSON_GetArraySize(arr);
    insert_ok = cJSON_InsertItemInArray(arr, 0, true_item);
    size_after = cJSON_GetArraySize(arr);
    got0 = cJSON_GetArrayItem(arr, 0);
    got1 = cJSON_GetArrayItem(arr, 1);
    first_is_true = cJSON_IsTrue(got0);
    second_is_string = cJSON_IsString(got1);

    // step 5: Validate
    cJSON_AddBoolToObject(root, "insert_ok", insert_ok);
    cJSON_AddBoolToObject(root, "first_is_true", first_is_true);
    cJSON_AddBoolToObject(root, "second_is_string", second_is_string);
    cJSON_AddNumberToObject(root, "size_before", (double)size_before);
    cJSON_AddNumberToObject(root, "size_after", (double)size_after);
    json_text = cJSON_PrintUnformatted(root);
    cJSON_free(json_text);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}