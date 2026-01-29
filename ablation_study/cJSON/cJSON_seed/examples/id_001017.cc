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
//<ID> 1017
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"items\":[{\"name\":\"alpha\",\"value\":1.5},{\"name\":\"beta\",\"value\":2.5}],\"meta\":{\"owner\":\"tester\"}}";
    size_t len = sizeof(json) - 1;
    cJSON *root = cJSON_ParseWithLength(json, len);

    // step 2: Configure
    cJSON *items = cJSON_GetObjectItem(root, "items");
    cJSON *first = cJSON_GetArrayItem(items, 0);
    char *first_name = cJSON_GetStringValue(cJSON_GetObjectItem(first, "name"));
    double first_value = cJSON_GetNumberValue(cJSON_GetObjectItem(first, "value"));
    cJSON *new_entry = cJSON_CreateObject();
    cJSON_AddItemToObject(new_entry, "copied_name", cJSON_CreateString(first_name));
    cJSON_AddItemToObject(new_entry, "copied_value", cJSON_CreateNumber(first_value));
    cJSON *extracted = cJSON_CreateArray();
    cJSON_AddItemToArray(extracted, new_entry);
    cJSON_AddItemToObject(root, "extracted", extracted);

    // step 3: Operate & Validate
    cJSON *detached = cJSON_DetachItemFromArray(items, 1);
    cJSON_AddItemToArray(extracted, detached);
    double second_value = cJSON_GetNumberValue(cJSON_GetObjectItem(detached, "value"));
    double sum = first_value + second_value;
    cJSON_AddNumberToObject(root, "sum", sum);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}