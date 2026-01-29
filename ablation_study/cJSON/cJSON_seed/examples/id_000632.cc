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
//<ID> 632
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(1.0);
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s1_dup = cJSON_Duplicate(s1, 1);

    // step 2: Configure
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, s1);
    cJSON_AddItemToArray(items, s1_dup);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddNumberToObject(root, "constant", 3.14);
    cJSON_AddStringToObject(root, "creator", "tester");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_items = cJSON_GetObjectItem(parsed, "items");
    int parsed_count = cJSON_GetArraySize(parsed_items);
    cJSON *first_item = cJSON_GetArrayItem(parsed_items, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    cJSON_DeleteItemFromObject(parsed, "constant");
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;count=%d;first=%.1f", version ? version : "null", parsed_count, first_value);
    cJSON_free(snapshot);

    // step 4: Cleanup
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}