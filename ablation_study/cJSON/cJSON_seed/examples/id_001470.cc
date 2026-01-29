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
//<ID> 1470
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *root2 = cJSON_CreateObject();
    cJSON *nested = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON *str = cJSON_CreateString("value");
    cJSON *to_delete = cJSON_CreateString("todelete");

    // step 2: Configure
    cJSON_AddItemToObject(nested, "nnum", num);
    cJSON_AddItemToObject(nested, "nstr", str);
    cJSON_AddItemToObject(root, "nested", cJSON_Duplicate(nested, 1));
    cJSON_AddItemToObject(root, "extra", cJSON_CreateTrue());
    cJSON_AddItemToObject(root, "remove_me", to_delete);
    cJSON_AddItemToObject(root2, "nested", cJSON_Duplicate(nested, 1));
    cJSON_AddItemToObject(root2, "extra", cJSON_CreateTrue());
    cJSON_AddItemToObject(root2, "remove_me", cJSON_CreateString("todelete"));

    // step 3: Operate & Validate
    cJSON *got_nested = cJSON_GetObjectItem(root, "nested");
    cJSON_bool is_obj = cJSON_IsObject(got_nested);
    char *printed_root = cJSON_PrintUnformatted(root);
    cJSON_bool equal_before = cJSON_Compare(root, root2, 1);
    cJSON *detached = cJSON_DetachItemFromObjectCaseSensitive(root2, "nested");
    cJSON_bool detached_is_obj = cJSON_IsObject(detached);
    cJSON_AddItemToObject(root2, "nested_copy", detached);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "remove_me");
    char *printed_root2 = cJSON_PrintUnformatted(root2);
    (void)is_obj;
    (void)equal_before;
    (void)detached_is_obj;

    // step 4: Cleanup
    cJSON_free(printed_root);
    cJSON_free(printed_root2);
    cJSON_Delete(nested);
    cJSON_Delete(root);
    cJSON_Delete(root2);
    // API sequence test completed successfully
    return 66;
}