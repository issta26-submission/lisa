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
//<ID> 334
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
    cJSON *first = (cJSON *)0;
    cJSON *second = (cJSON *)0;
    cJSON *copied = (cJSON *)0;
    cJSON_bool first_is_string = (cJSON_bool)0;
    cJSON_bool root_is_object = (cJSON_bool)0;
    const char *got_str = (const char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", arr);

    // step 3: Configure
    str_item = cJSON_CreateString("example_string");
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToArray(arr, true_item);

    // step 4: Operate
    first = cJSON_GetArrayItem(arr, 0);
    second = cJSON_GetArrayItem(arr, 1);
    first_is_string = cJSON_IsString(first);
    root_is_object = cJSON_IsObject(root);
    got_str = cJSON_GetStringValue(first);

    // step 5: Validate
    copied = cJSON_AddStringToObject(root, "copied", got_str);
    (void)second;
    (void)first_is_string;
    (void)root_is_object;
    (void)copied;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}