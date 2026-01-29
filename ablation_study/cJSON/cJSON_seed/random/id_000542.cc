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
//<ID> 542
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
    cJSON *null_item = (cJSON *)0;
    cJSON *detached = (cJSON *)0;
    cJSON *first_elem = (cJSON *)0;
    cJSON_bool added1 = 0;
    cJSON_bool added2 = 0;
    cJSON_bool added3 = 0;
    cJSON_bool is_array_before = 0;
    cJSON_bool is_array_after = 0;
    const char *first_str = (const char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("example-string");
    null_item = cJSON_CreateNull();

    // step 3: Configure
    added1 = cJSON_AddItemToArray(arr, str_item);
    added2 = cJSON_AddItemToArray(arr, null_item);
    added3 = cJSON_AddItemToObject(root, "payload", arr);

    // step 4: Operate
    is_array_before = cJSON_IsArray(cJSON_GetObjectItem(root, "payload"));
    detached = cJSON_DetachItemViaPointer(root, cJSON_GetObjectItem(root, "payload"));
    cJSON_AddItemToObject(root, "reattached", detached);

    // step 5: Validate
    is_array_after = cJSON_IsArray(cJSON_GetObjectItem(root, "reattached"));
    first_elem = cJSON_GetArrayItem(cJSON_GetObjectItem(root, "reattached"), 0);
    first_str = cJSON_GetStringValue(first_elem);

    (void)added1;
    (void)added2;
    (void)added3;
    (void)is_array_before;
    (void)is_array_after;
    (void)first_str;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}