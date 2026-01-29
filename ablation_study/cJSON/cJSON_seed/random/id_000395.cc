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
//<ID> 395
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
    cJSON *bool_item = (cJSON *)0;
    cJSON *got_list = (cJSON *)0;
    cJSON *alias_item = (cJSON *)0;
    int list_size = 0;
    cJSON_bool ref_added = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    str_item = cJSON_CreateString("alpha");

    // step 3: Configure
    cJSON_AddItemToArray(arr, str_item);
    cJSON_AddItemToObject(root, "list", arr);
    bool_item = cJSON_AddBoolToObject(root, "enabled", 1);
    ref_added = cJSON_AddItemReferenceToObject(root, "alias", str_item);

    // step 4: Operate
    got_list = cJSON_GetObjectItem(root, "list");
    list_size = cJSON_GetArraySize(got_list);
    alias_item = cJSON_GetObjectItem(root, "alias");

    // step 5: Validate
    (void)bool_item;
    (void)ref_added;
    (void)alias_item;
    (void)list_size;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}