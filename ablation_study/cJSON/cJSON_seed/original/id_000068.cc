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
//<ID> 68
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
    cJSON_AddNumberToObject(child, "id", 123.0);
    cJSON_AddStringToObject(child, "name", "node");
    cJSON_AddItemToObject(root, "original", child);
    cJSON *refs = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "refs", refs);

    // step 2: Configure
    cJSON *ref = cJSON_CreateObjectReference(child);
    cJSON_bool added = cJSON_AddItemReferenceToArray(refs, ref);

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(refs, 0);
    cJSON_bool first_is_obj = cJSON_IsObject(first);
    cJSON_bool equal = cJSON_Compare(first, child, 1);
    cJSON *id_item = cJSON_GetObjectItem(first, "id");
    double id_val = cJSON_GetNumberValue(id_item);
    int summary = (int)added + (int)first_is_obj + (int)equal + (int)id_val;
    (void)summary;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}