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
//<ID> 174
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
    cJSON *insert_true = (cJSON *)0;
    cJSON *tmp0 = (cJSON *)0;
    cJSON *tmp1 = (cJSON *)0;
    cJSON *json_text = (cJSON *)0;
    cJSON_bool first_is_string = (cJSON_bool)0;
    cJSON_bool second_is_true = (cJSON_bool)0;
    char *printed = (char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    insert_true = cJSON_CreateTrue();

    // step 3: Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "label", cJSON_CreateString("sequence_test"));
    cJSON_AddBoolToObject(root, "always_true_flag", 1);

    // step 4: Operate
    cJSON_AddItemToArray(arr, cJSON_CreateString("zero"));
    cJSON_AddItemToArray(arr, cJSON_CreateString("two"));
    cJSON_InsertItemInArray(arr, 1, insert_true);

    // step 5: Validate
    tmp0 = cJSON_GetArrayItem(arr, 0);
    tmp1 = cJSON_GetArrayItem(arr, 1);
    first_is_string = cJSON_IsString(tmp0);
    second_is_true = cJSON_IsTrue(tmp1);
    cJSON_AddBoolToObject(root, "first_is_string", first_is_string);
    cJSON_AddBoolToObject(root, "second_is_true", second_is_true);
    printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}