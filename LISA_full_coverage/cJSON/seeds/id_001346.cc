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
//<ID> 1346
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json1 = "{\"arr\":[10,20,30],\"obj\":{\"keep\":1}}";
    cJSON *root1 = cJSON_Parse(json1);
    const char *json2 = "{\"trailing\":true} trailing";
    const char *parse_end = NULL;
    cJSON *root2 = cJSON_ParseWithOpts(json2, &parse_end, 0);

    // step 2: Configure
    cJSON *arr = cJSON_GetObjectItem(root1, "arr");
    cJSON_AddNullToObject(root1, "new_null");
    cJSON_AddItemToObject(root1, "other", root2);

    // step 3: Operate
    cJSON *replacement = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON_DeleteItemFromObjectCaseSensitive(root1, "obj");

    // step 4: Validate & Cleanup
    char *printed = cJSON_PrintUnformatted(root1);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root1);
    // API sequence test completed successfully
    return 66;
}