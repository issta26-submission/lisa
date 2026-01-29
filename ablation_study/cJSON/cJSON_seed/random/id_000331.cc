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
//<ID> 331
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
    char *got_str = (char *)0;
    cJSON_bool is_str = (cJSON_bool)0;
    cJSON_bool is_obj = (cJSON_bool)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();

    // step 3: Configure
    cJSON_AddItemToObject(root, "items", arr);
    str_item = cJSON_CreateString("example_string");
    cJSON_AddItemToArray(arr, str_item);
    true_item = cJSON_CreateTrue();
    cJSON_AddItemToArray(arr, true_item);

    // step 4: Operate
    first = cJSON_GetArrayItem(arr, 0);
    got_str = cJSON_GetStringValue(first);
    is_str = cJSON_IsString(first);
    is_obj = cJSON_IsObject(root);

    // step 5: Validate
    cJSON_AddItemToObject(root, "first_copy", cJSON_CreateString(got_str));
    (void)is_str;
    (void)is_obj;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}