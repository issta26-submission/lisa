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
//<ID> 1349
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json1 = "{\"arr\":[1,2,3],\"x\":null}";
    cJSON *parsed1 = cJSON_Parse(json1);
    const char *json2 = "{\"arr\":[10,20,30],\"keep\":true} trailing";
    const char *parse_end = NULL;
    cJSON *parsed2 = cJSON_ParseWithOpts(json2, &parse_end, 0);

    // step 2: Configure
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "p1", parsed1);
    cJSON_AddItemToObject(root, "p2", parsed2);
    cJSON *extras = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "extras", extras);

    // step 3: Operate
    cJSON *arr = cJSON_GetObjectItem(parsed1, "arr");
    cJSON *newnum = cJSON_CreateNumber(99.0);
    cJSON_bool replaced = cJSON_ReplaceItemInArray(arr, 1, newnum);
    cJSON *marker2 = cJSON_CreateNumber((double)replaced);
    cJSON_AddItemToArray(extras, marker2);
    cJSON_AddNullToObject(root, "marker");
    cJSON_DeleteItemFromObjectCaseSensitive(root, "p2");

    // step 4: Validate & Cleanup
    char *printed = cJSON_PrintUnformatted(root);
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