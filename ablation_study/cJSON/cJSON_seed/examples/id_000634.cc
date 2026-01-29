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
//<ID> 634
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
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(items, n1);
    cJSON_AddItemToArray(items, n2);
    cJSON_AddItemToObject(root, "items", items);
    cJSON_AddStringToObject(root, "creator", "fuzzer");

    // step 2: Configure
    char *snapshot = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(256);
    memset(buffer, 0, 256);
    sprintf((char *)buffer, "%s", snapshot ? snapshot : "");

    // step 3: Operate & Validate
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_items = cJSON_GetObjectItem(parsed, "items");
    cJSON *first_item = cJSON_GetArrayItem(parsed_items, 0);
    cJSON *dup_item = cJSON_Duplicate(first_item, 1);
    cJSON_AddItemToArray(parsed_items, dup_item);
    cJSON_DeleteItemFromObject(parsed, "items");
    char status[256];
    memset(status, 0, sizeof(status));
    cJSON *parsed_creator = cJSON_GetObjectItem(parsed, "creator");
    const char *creator_name = cJSON_GetStringValue(parsed_creator);
    sprintf(status, "ver=%s;creator=%s", version ? version : "null", creator_name ? creator_name : "null");

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}