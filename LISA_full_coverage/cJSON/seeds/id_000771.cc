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
//<ID> 771
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    char json_text[] = "{\"arr\":[1,2,3],\"name\":\"example\"}";
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(json_text, (size_t)(sizeof(json_text) - 1), &parse_end, 1);
    cJSON *extra_array = cJSON_CreateArray();
    cJSON_AddItemToArray(extra_array, cJSON_CreateNumber(10));
    cJSON_AddItemToArray(extra_array, cJSON_CreateString("hello"));
    cJSON_AddItemToObject(root, "extra", extra_array);

    // step 2: Configure
    cJSON_AddRawToObject(root, "raw_blob", "{\"inner\":true}");
    cJSON_AddNullToObject(root, "maybe_null");

    // step 3: Operate
    cJSON *arr = cJSON_GetObjectItemCaseSensitive(root, "arr");
    cJSON *detached = cJSON_DetachItemFromArray(arr, 1);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(detached);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}