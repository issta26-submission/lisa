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
//<ID> 1341
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text1 = "{\"arr\":[1,2,3],\"remove\":\"me\",\"flag\":true}";
    cJSON *parsed = cJSON_Parse(json_text1);
    const char *json_text2 = "{\"extra\":123} trailing";
    const char *parse_end = NULL;
    cJSON *parsed2 = cJSON_ParseWithOpts(json_text2, &parse_end, 0);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "source", parsed);
    cJSON_AddItemToObject(root, "attach", parsed2);
    cJSON_AddNullToObject(root, "added_null");
    cJSON *arr = cJSON_GetObjectItem(parsed, "arr");
    cJSON *newnum = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemInArray(arr, 1, newnum);

    // step 3: Operate
    cJSON_DeleteItemFromObjectCaseSensitive(parsed, "remove");
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}