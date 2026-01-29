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
//<ID> 1279
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
    cJSON *str_a = cJSON_CreateString("alpha");
    cJSON *str_b = cJSON_CreateString("beta");

    // step 2: Configure
    cJSON_AddItemToArray(items, str_a);
    cJSON_AddItemToArray(items, str_b);
    cJSON_AddItemToObject(root, "items", items);
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddNullToObject(meta, "none");
    cJSON *label = cJSON_CreateString("dataset");
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(items);
    (void)count;
    cJSON *inner = cJSON_AddObjectToObject(meta, "inner");
    cJSON_AddItemToObject(inner, "description", cJSON_CreateString("two elements"));

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}