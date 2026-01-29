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
//<ID> 1345
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json1 = "{\"arr\":[10,20,30],\"keep\":true}";
    cJSON *parsed1 = cJSON_Parse(json1);

    // step 2: Configure
    cJSON *arr = cJSON_GetObjectItem(parsed1, "arr");
    cJSON *newnum = cJSON_CreateNumber(99.0);
    cJSON_ReplaceItemInArray(arr, 1, newnum);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "source", parsed1);
    cJSON *marker = cJSON_AddNullToObject(root, "removed_marker");

    // step 3: Operate
    const char *json2 = "{\"alpha\":\"beta\"} trailing";
    const char *parse_end = NULL;
    cJSON *parsed2 = cJSON_ParseWithOpts(json2, &parse_end, 0);
    cJSON_AddItemToObject(root, "extra", parsed2);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "removed_marker");

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