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
//<ID> 1381
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json = "{\"arr\":[\"a\",\"b\",\"c\"],\"flag\":false} trailing";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json, &parse_end, 0);

    // step 2: Configure
    cJSON *arr = cJSON_GetObjectItem(parsed, "arr");
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInArray(arr, 1, replacement);
    cJSON *true_item = cJSON_CreateTrue();
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "original", parsed);
    cJSON_AddItemToObject(root, "status", true_item);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);

    // step 4: Validate & Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}