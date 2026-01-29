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
//<ID> 396
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
    cJSON *s1 = cJSON_CreateString("first");
    cJSON *s2 = cJSON_CreateString("second");
    cJSON *t = cJSON_CreateTrue();
    cJSON *ins = cJSON_CreateString("to_insert");

    // step 2: Configure
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s2);
    cJSON_AddItemToArray(items, t);
    cJSON_InsertItemInArray(items, 1, ins);
    cJSON_AddItemToObject(root, "items", items);

    // step 3: Operate and Validate
    cJSON *det = cJSON_DetachItemFromArray(items, 2);
    char *new_val = cJSON_SetValuestring(det, "modified-second");
    (void)new_val;
    cJSON_AddItemToArray(items, det);
    int count = cJSON_GetArraySize(items);
    const char *first_val = cJSON_GetStringValue(cJSON_GetArrayItem(items, 0));
    (void)first_val;
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}