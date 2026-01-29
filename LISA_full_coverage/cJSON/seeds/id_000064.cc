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
//<ID> 64
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
    cJSON *entry = cJSON_CreateObject();
    cJSON_AddItemToObject(entry, "id", cJSON_CreateNumber(123.0));
    cJSON_AddItemToObject(entry, "active", cJSON_CreateTrue());
    cJSON_AddItemToObject(entry, "name", cJSON_CreateString("alpha"));
    cJSON_AddItemToObject(root, "original", entry);

    // step 2: Configure
    cJSON *entry_ref = cJSON_CreateObjectReference(entry);
    cJSON_bool added_ref = cJSON_AddItemReferenceToArray(items, entry_ref);
    cJSON_AddItemToArray(items, cJSON_CreateNumber(9.0));

    // step 3: Operate and Validate
    cJSON *first_item = cJSON_GetArrayItem(items, 0);
    cJSON *second_item = cJSON_GetArrayItem(items, 1);
    cJSON_bool first_is_object = cJSON_IsObject(first_item);
    cJSON_bool equal_ref = cJSON_Compare(first_item, entry, 1);
    cJSON *id_item = cJSON_GetObjectItem(first_item, "id");
    double id_val = cJSON_GetNumberValue(id_item);
    int summary = (int)id_val + (int)first_is_object + (int)equal_ref + (int)added_ref + (int)cJSON_GetNumberValue(second_item);
    cJSON_AddNumberToObject(root, "summary", (double)summary);

    // step 4: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}