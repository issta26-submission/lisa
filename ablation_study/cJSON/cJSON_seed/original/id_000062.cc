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
//<ID> 62
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddStringToObject(child, "name", "child1");
    cJSON_AddItemToObject(root, "child", child);
    cJSON *refs = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "refs", refs);

    // step 2: Configure
    cJSON *child_ref = cJSON_CreateObjectReference(child);
    cJSON_bool added_ref = cJSON_AddItemReferenceToArray(refs, child_ref);

    // step 3: Operate and Validate
    cJSON *first_in_array = cJSON_GetArrayItem(refs, 0);
    cJSON_bool equal = cJSON_Compare(first_in_array, child, 1);
    cJSON_bool is_obj = cJSON_IsObject(first_in_array);
    int summary = (int)added_ref + (int)equal + (int)is_obj;
    (void)summary;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}