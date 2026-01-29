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
//<ID> 117
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
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(items, n1);
    cJSON *n2 = cJSON_CreateNumber(2.71);
    cJSON_AddItemToArray(items, n2);

    // step 2: Configure
    cJSON *backup = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "backup", backup);
    cJSON_AddNumberToObject(root, "initialized", 1.0);

    // step 3: Operate and Validate
    cJSON *got_items = cJSON_GetObjectItem(root, "items");
    cJSON *first = cJSON_GetArrayItem(got_items, 0);
    double v0 = cJSON_GetNumberValue(first);
    cJSON *dup_first = cJSON_Duplicate(first, 0);
    cJSON_AddItemToArray(backup, dup_first);
    cJSON *second = cJSON_GetArrayItem(got_items, 1);
    double v1 = cJSON_GetNumberValue(second);
    double sum = v0 + v1;
    cJSON_AddNumberToObject(root, "sum", sum);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}