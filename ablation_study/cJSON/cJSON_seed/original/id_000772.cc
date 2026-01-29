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
//<ID> 772
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json_text[] = "{\"arr\":[10,20,30],\"name\":\"orig\"}";
    size_t json_len = sizeof(json_text) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json_text, json_len, &parse_end, 1);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    cJSON_SetValuestring(name_item, "updated");
    cJSON *raw_added = cJSON_AddRawToObject(root, "raw", "{\"nested\":true}");
    cJSON *null_added = cJSON_AddNullToObject(root, "maybe");

    // step 3: Operate
    cJSON *arr = cJSON_GetObjectItemCaseSensitive(root, "arr");
    cJSON *detached = cJSON_DetachItemFromArray(arr, 1);
    cJSON *newnum = cJSON_CreateNumber(99.0);
    cJSON_AddItemToArray(arr, newnum);
    cJSON_AddItemToObject(root, "detached", detached);

    // step 4: Validate & Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}