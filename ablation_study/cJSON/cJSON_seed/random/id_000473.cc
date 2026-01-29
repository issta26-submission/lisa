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
//<ID> 473
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
    cJSON *got_item = (cJSON *)0;
    cJSON_bool is_raw = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("sample");

    // step 3: Configure
    cJSON_AddNullToObject(root, "empty");
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "data", arr);

    // step 4: Operate
    got_item = cJSON_GetArrayItem(arr, 0);
    is_raw = cJSON_IsRaw(got_item);

    // step 5: Validate
    (void)is_raw;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}