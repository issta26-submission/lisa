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
//<ID> 545
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *obj1 = cJSON_CreateObject();
    cJSON *name1 = cJSON_CreateString("alpha");
    cJSON_AddItemToObject(obj1, "name", name1);
    cJSON *active1 = cJSON_CreateTrue();
    cJSON_AddItemToObject(obj1, "active", active1);
    cJSON_AddItemToArray(items, obj1);
    cJSON *obj2 = cJSON_CreateObject();
    cJSON *name2 = cJSON_CreateString("beta");
    cJSON_AddItemToObject(obj2, "name", name2);
    cJSON *active2 = cJSON_CreateFalse();
    cJSON_AddItemToObject(obj2, "active", active2);
    cJSON_AddItemToArray(items, obj2);

    // step 2: Configure
    cJSON *detached_items = cJSON_DetachItemFromObject(root, "items");
    cJSON *first_elem = cJSON_GetArrayItem(detached_items, 0);
    cJSON *first_name_item = cJSON_GetObjectItem(first_elem, "name");
    char *first_name_val = cJSON_GetStringValue(first_name_item);
    cJSON *copied_name = cJSON_CreateString(first_name_val);
    cJSON_AddItemToObject(root, "copied_first_name", copied_name);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON *refs = cJSON_CreateArray();
    cJSON *ref_label = cJSON_CreateString("ref_to_first");
    cJSON_AddItemToArray(refs, ref_label);
    cJSON_AddItemToObject(root, "refs", refs);
    cJSON *retrieved = cJSON_GetObjectItem(root, "copied_first_name");
    char *retrieved_val = cJSON_GetStringValue(retrieved);
    cJSON *confirm = cJSON_CreateString(retrieved_val);
    cJSON_AddItemToArray(refs, confirm);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_Delete(detached_items);
    // API sequence test completed successfully
    return 66;
}