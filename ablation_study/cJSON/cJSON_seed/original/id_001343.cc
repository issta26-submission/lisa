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
//<ID> 1343
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_with_trailing = "{\"arr\":[10,20,30],\"name\":\"example\"} trailing";
    const char *parse_end = NULL;
    cJSON *parsed_with_trailing = cJSON_ParseWithOpts(json_with_trailing, &parse_end, 0);
    const char *json_simple = "{\"key\":null,\"list\":[1,2,3]}";
    cJSON *parsed_simple = cJSON_Parse(json_simple);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "parsed_simple", parsed_simple);
    cJSON_AddItemToObject(root, "parsed_with_trailing", parsed_with_trailing);
    cJSON *nullable = cJSON_AddNullToObject(root, "nullable_marker");
    cJSON *list = cJSON_GetObjectItem(parsed_simple, "list");
    cJSON *replacement = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemInArray(list, 1, replacement);

    // step 3: Operate
    cJSON_DeleteItemFromObjectCaseSensitive(root, "parsed_with_trailing");
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);

    // step 4: Validate & Cleanup
    cJSON_Delete(root);
    cJSON_free(buf);
    // API sequence test completed successfully
    return 66;
}