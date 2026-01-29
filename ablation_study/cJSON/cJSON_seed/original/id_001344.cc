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
//<ID> 1344
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json1 = "{\"arr\":[1,2,3],\"obj\":{\"key\":\"value\"}}";
    const char *json2 = "{\"arr\":[10,20,30],\"leaf\":\"to_remove\"} trailing";
    const char *parse_end = NULL;
    cJSON *parsed1 = cJSON_Parse(json1);
    cJSON *parsed2 = cJSON_ParseWithOpts(json2, &parse_end, 0);

    // step 2: Configure
    cJSON *arr1 = cJSON_GetObjectItem(parsed1, "arr");
    cJSON *arr2 = cJSON_GetObjectItem(parsed2, "arr");
    cJSON *src_item = cJSON_GetArrayItem(arr2, 1);
    cJSON *dup_item = cJSON_Duplicate(src_item, 1);
    cJSON_ReplaceItemInArray(arr1, 1, dup_item);
    cJSON *obj = cJSON_GetObjectItem(parsed1, "obj");
    cJSON_AddNullToObject(obj, "added_null");
    cJSON_DeleteItemFromObjectCaseSensitive(parsed2, "leaf");

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(parsed1);
    size_t printed_len = strlen(printed);
    char *buf = (char *)cJSON_malloc(printed_len + 1);
    memset(buf, 0, printed_len + 1);
    memcpy(buf, printed, printed_len + 1);
    cJSON_Minify(buf);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(parsed1);
    cJSON_Delete(parsed2);

    // API sequence test completed successfully
    return 66;
}