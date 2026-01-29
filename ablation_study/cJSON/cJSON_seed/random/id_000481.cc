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
//<ID> 481
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
    cJSON *true_item = (cJSON *)0;
    cJSON *got_name = (cJSON *)0;
    char *printed = (char *)0;
    const char *name_value = (const char *)0;
    cJSON_bool arr_check = 0;
    cJSON_bool truth_check = 0;
    int arr_size = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    true_item = cJSON_CreateTrue();

    // step 3: Configure
    cJSON_AddStringToObject(root, "name", "example");
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "always_true", true_item);
    cJSON_AddFalseToObject(root, "always_false");
    cJSON_AddBoolToObject(root, "custom_flag", (cJSON_bool)1);

    // step 4: Operate
    got_name = cJSON_GetObjectItem(root, "name");
    name_value = cJSON_GetStringValue(got_name);
    arr_check = cJSON_IsArray(cJSON_GetObjectItem(root, "items"));
    truth_check = cJSON_IsTrue(cJSON_GetObjectItem(root, "always_true"));
    arr_size = cJSON_GetArraySize(arr);

    // step 5: Validate
    printed = cJSON_PrintUnformatted(root);
    cJSON_Minify(printed);
    (void)name_value;
    (void)arr_check;
    (void)truth_check;
    (void)arr_size;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}