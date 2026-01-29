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
//<ID> 773
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"p\":1,\"q\":[\"a\",\"b\",\"c\"]}";
    size_t json_len = (sizeof(json_text) - 1);
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json_text, json_len, &parse_end, 1);
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *first = cJSON_CreateString("one");
    cJSON *second = cJSON_CreateString("two");
    cJSON *third = cJSON_CreateString("three");
    cJSON_AddItemToArray(items, first);
    cJSON_AddItemToArray(items, second);
    cJSON_AddItemToArray(items, third);

    // step 2: Configure
    cJSON *raw_item = cJSON_AddRawToObject(root, "rawdata", "{\"inner\":123}");
    cJSON *null_item = cJSON_AddNullToObject(root, "maybe");
    cJSON *items_ptr = cJSON_GetObjectItemCaseSensitive(root, "items");
    cJSON *q_array = cJSON_GetObjectItemCaseSensitive(parsed, "q");

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromArray(items_ptr, 1);
    cJSON_AddItemToArray(q_array, detached);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(parsed);

    // API sequence test completed successfully
    return 66;
}