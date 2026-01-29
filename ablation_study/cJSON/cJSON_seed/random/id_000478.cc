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
//<ID> 478
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
    cJSON *first_item = (cJSON *)0;
    cJSON *status_str = (cJSON *)0;
    cJSON_bool is_raw = 0;
    const char *labels[2] = { "raw_false", "raw_true" };

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("example_value");

    // step 3: Configure
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddNullToObject(root, "optional");

    // step 4: Operate
    first_item = cJSON_GetArrayItem((const cJSON *)arr, 0);
    is_raw = cJSON_IsRaw((const cJSON *const)first_item);

    // step 5: Validate
    status_str = cJSON_CreateString(labels[(int)is_raw]);
    cJSON_AddItemToObject(root, "status", status_str);

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}