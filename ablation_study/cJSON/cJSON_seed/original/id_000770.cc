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
//<ID> 770
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json_text[] = "{\"arr\":[1,2,3],\"obj\":{\"a\":\"b\"}}";
    size_t buffer_length = (size_t)(sizeof(json_text) - 1);
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json_text, buffer_length, &parse_end, 1);
    char temp[16];
    memset(temp, 0, sizeof(temp));
    cJSON *raw_added = cJSON_AddRawToObject(root, "raw_inner", "{\"x\":true}");
    cJSON *null_added = cJSON_AddNullToObject(root, "explicit_null");

    // step 2: Configure
    cJSON *arr = cJSON_GetObjectItemCaseSensitive(root, "arr");
    cJSON *new_number = cJSON_CreateNumber(99.0);
    cJSON_AddItemToArray(arr, new_number);

    // step 3: Operate
    cJSON *detached = cJSON_DetachItemFromArray(arr, 1);
    cJSON *obj = cJSON_GetObjectItemCaseSensitive(root, "obj");
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}