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
//<ID> 1417
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "version", 1.2);
    cJSON_AddBoolToObject(root, "active", 1);
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *num_item = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(items, num_item);

    // step 2: Configure
    cJSON *entry = cJSON_CreateObject();
    cJSON_AddNumberToObject(entry, "score", 88.5);
    cJSON_AddBoolToObject(entry, "passed", 1);
    cJSON_AddItemToArray(items, entry);
    cJSON *dup_entry = cJSON_Duplicate(entry, 1);
    cJSON_AddItemToArray(items, dup_entry);
    cJSON_AddItemReferenceToArray(items, entry);

    // step 3: Operate & Validate
    cJSON *items_ptr = cJSON_GetObjectItem(root, "items");
    cJSON *first = cJSON_GetArrayItem(items_ptr, 0);
    double first_val = cJSON_GetNumberValue(first);
    cJSON_SetNumberHelper(first, first_val + 10.0);
    cJSON *ver = cJSON_GetObjectItem(root, "version");
    double ver_val = cJSON_GetNumberValue(ver);
    (void)ver_val; // use value to show data flow
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_AddRawToObject(root, "snapshot", snapshot);
    cJSON_free((void *)snapshot);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}