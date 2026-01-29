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
//<ID> 774
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"numbers\":[10,20,30],\"meta\":{\"name\":\"example\"}}";
    size_t json_len = sizeof(json_text) - 1;
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithLengthOpts(json_text, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *meta = cJSON_GetObjectItemCaseSensitive(parsed, "meta");
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(meta, "name");
    const char *name_str = cJSON_GetStringValue(name_item);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "source_name", name_str);
    cJSON_AddRawToObject(root, "embedded", "{\"inner\":true}");
    cJSON_AddNullToObject(root, "maybe");

    // step 3: Operate
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(2));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(3));
    cJSON_AddItemToObject(root, "list", arr);
    cJSON *fetched_list = cJSON_GetObjectItemCaseSensitive(root, "list");
    cJSON *detached = cJSON_DetachItemFromArray(fetched_list, 1);
    cJSON_AddItemToObject(parsed, "extracted", detached);

    // step 4: Validate & Cleanup
    cJSON_Delete(root);
    cJSON_Delete(parsed);
    // API sequence test completed successfully
    return 66;
}