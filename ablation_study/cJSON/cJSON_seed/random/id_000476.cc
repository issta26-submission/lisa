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
//<ID> 476
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
    cJSON *item0 = (cJSON *)0;
    char *printed = (char *)0;
    cJSON_bool is_raw = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("alpha");

    // step 3: Configure
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "list", arr);
    null_item = cJSON_AddNullToObject(root, "unused");

    // step 4: Operate
    item0 = cJSON_GetArrayItem((const cJSON *)arr, 0);
    is_raw = cJSON_IsRaw((const cJSON *const)item0);

    // step 5: Validate
    printed = cJSON_PrintUnformatted((const cJSON *)root);
    (void)is_raw;
    (void)null_item;
    (void)item0;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}