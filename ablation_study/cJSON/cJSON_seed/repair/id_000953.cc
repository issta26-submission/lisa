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
//<ID> 953
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
    cJSON *n1 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(items, n1);
    cJSON *n2 = cJSON_CreateNumber(4.5);
    cJSON_AddItemToArray(items, n2);
    cJSON *t = cJSON_CreateTrue();
    cJSON_AddItemToArray(items, t);

    // step 2: Configure
    cJSON *sub = cJSON_CreateObject();
    cJSON_AddStringToObject(sub, "name", "subnode");
    cJSON_AddItemToObject(root, "sub", sub);
    cJSON *sub_ref = cJSON_CreateObjectReference(sub);
    cJSON *extra = cJSON_CreateNumber(10.0);
    cJSON_AddItemToObject(root, "extra", extra);

    // step 3: Operate and Validate
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    double first_val = cJSON_GetNumberValue(first_item);
    cJSON_SetNumberHelper(first_item, first_val + 1.0);
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    double second_val = cJSON_GetNumberValue(second_item);
    char *snapshot = cJSON_PrintUnformatted(root);
    char *ref_snapshot = cJSON_PrintUnformatted(sub_ref);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(ref_snapshot);
    cJSON_Delete(sub_ref);
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)second_val;
    return 66;
}