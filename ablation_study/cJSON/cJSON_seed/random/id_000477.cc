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
//<ID> 477
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
    cJSON *status_str = (cJSON *)0;
    cJSON *got_item = (cJSON *)0;
    cJSON_bool is_raw = 0;
    const char *status = (const char *)0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("example_raw_candidate");

    // step 3: Configure
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddNullToObject(root, "null_entry");

    // step 4: Operate
    got_item = cJSON_GetArrayItem(arr, 0);
    is_raw = cJSON_IsRaw(got_item);
    status = is_raw ? "item_is_raw" : "item_not_raw";
    status_str = cJSON_CreateString(status);
    cJSON_AddItemToObject(root, "raw_status", status_str);

    // step 5: Validate
    (void)got_item;
    (void)is_raw;
    (void)status;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}