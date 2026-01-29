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
//<ID> 1348
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"arr\":[10,20,30],\"key\":null} trailing";
    const char *parse_end = NULL;
    cJSON *parsed = cJSON_ParseWithOpts(json_text, &parse_end, 0);
    const char *simple_text = "{\"a\":1}";
    cJSON *simple_parsed = cJSON_Parse(simple_text);

    // step 2: Configure
    cJSON *arr = cJSON_GetObjectItem(parsed, "arr");
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "parsed", parsed);
    cJSON_AddItemToObject(root, "simple", simple_parsed);
    cJSON_AddNullToObject(simple_parsed, "new_null");
    cJSON_DeleteItemFromObjectCaseSensitive(parsed, "key");
    cJSON *new_num = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemInArray(arr, 1, new_num);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}