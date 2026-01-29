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
//<ID> 533
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

    // step 2: Configure
    cJSON_AddStringToObject(root, "title", "example");
    cJSON_AddNullToObject(root, "optional");
    cJSON *s0 = cJSON_CreateString("first");
    cJSON *s1 = cJSON_CreateString("second");
    cJSON_AddItemToArray(items, s0);
    cJSON_AddItemToArray(items, s1);

    // step 3: Operate & Validate
    cJSON_bool is_items_array = cJSON_IsArray(items);
    int size_before = cJSON_GetArraySize(items);
    char *snapshot1 = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot1);
    cJSON *parsed_items = cJSON_GetObjectItem(parsed, "items");
    cJSON_bool is_parsed_array = cJSON_IsArray(parsed_items);
    int parsed_size = cJSON_GetArraySize(parsed_items);
    cJSON_DeleteItemFromArray(items, 1);
    int size_after = cJSON_GetArraySize(items);
    char *snapshot2 = cJSON_PrintUnformatted(root);
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf, "is_items_array=%d;is_parsed_array=%d;parsed_size=%d;size_before=%d;size_after=%d",
            (int)is_items_array, (int)is_parsed_array, parsed_size, size_before, size_after);

    // step 4: Cleanup
    cJSON_free(snapshot1);
    cJSON_free(snapshot2);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}