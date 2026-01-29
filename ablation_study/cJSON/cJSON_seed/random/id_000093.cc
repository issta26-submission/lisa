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
//<ID> 93
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
    cJSON *s_item = (cJSON *)0;
    cJSON *f_item = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    const char *detached_str = (const char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    s_item = cJSON_CreateString("detached_string");
    f_item = cJSON_CreateFalse();

    // step 3: Configure
    cJSON_AddItemToArray(arr, s_item);
    cJSON_AddItemToArray(arr, f_item);
    cJSON_AddItemToObject(root, "array", arr);

    // step 4: Operate
    detached = cJSON_DetachItemFromArray(arr, 0);
    detached_str = cJSON_GetStringValue(detached);
    cJSON_AddItemToObject(root, "first_value", cJSON_CreateString(detached_str));

    // step 5: Validate
    cJSON_DeleteItemFromObject(root, "array");

    // step 6: Cleanup
    cJSON_Delete(detached);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}