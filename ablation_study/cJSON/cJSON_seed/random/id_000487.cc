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
//<ID> 487
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *meta = (cJSON *)0;
    cJSON *items = (cJSON *)0;
    cJSON *true_item = (cJSON *)0;
    cJSON *added_name = (cJSON *)0;
    cJSON *added_flag = (cJSON *)0;
    cJSON *added_disabled = (cJSON *)0;
    cJSON *got_name = (cJSON *)0;
    const char *name_value = (const char *)0;
    char *printed = (char *)0;
    cJSON_bool items_is_array = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    meta = cJSON_CreateObject();
    items = cJSON_CreateArray();
    true_item = cJSON_CreateTrue();

    // step 3: Configure
    added_name = cJSON_AddStringToObject(root, "name", "cJSON_test");
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "items", items);
    added_flag = cJSON_AddBoolToObject(meta, "flag", (cJSON_bool)1);
    added_disabled = cJSON_AddFalseToObject(meta, "disabled");
    cJSON_AddItemToArray(items, true_item);

    // step 4: Operate
    items_is_array = cJSON_IsArray(items);
    got_name = cJSON_GetObjectItem(root, "name");
    name_value = cJSON_GetStringValue(got_name);

    // step 5: Validate
    printed = cJSON_Print(root);
    (void)added_name;
    (void)added_flag;
    (void)added_disabled;
    (void)name_value;
    (void)items_is_array;

    // step 6: Cleanup
    cJSON_free((void *)printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}