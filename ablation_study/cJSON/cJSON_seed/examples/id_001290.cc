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
//<ID> 1290
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
    cJSON *str = cJSON_CreateString("alpha");
    cJSON *t = cJSON_CreateTrue();

    // step 2: Configure
    cJSON_AddItemToArray(items, str);
    cJSON_AddItemToArray(items, t);
    cJSON_AddItemToObject(root, "items", items);
    cJSON *label = cJSON_AddStringToObject(root, "label", "dataset");
    cJSON_AddItemReferenceToObject(root, "label_ref", label);
    cJSON *items_dup = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_dup", items_dup);

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(items);
    (void)count;
    cJSON_bool is_true = cJSON_IsTrue(t);
    (void)is_true;
    const char *val = cJSON_GetStringValue(str);
    (void)val;

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}