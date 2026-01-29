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
//<ID> 750
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON *nullable = cJSON_CreateNull();
    cJSON *name_item = cJSON_CreateString("tester");
    cJSON *count_item = cJSON_CreateNumber(1.0);

    // step 2: Configure
    cJSON_AddItemToObject(meta, "name", name_item);
    cJSON_AddItemToObject(meta, "count", count_item);
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(root, "optional", nullable);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *found_meta = cJSON_GetObjectItem(parsed, "meta");
    cJSON *found_name = cJSON_GetObjectItem(found_meta, "name");
    const char *extracted_name = cJSON_GetStringValue(found_name);
    cJSON_bool optional_is_null = cJSON_IsNull(cJSON_GetObjectItem(parsed, "optional"));
    cJSON_bool meta_exists = cJSON_HasObjectItem(parsed, "meta");
    cJSON *dup = cJSON_Duplicate(parsed, 1);
    cJSON_bool equal = cJSON_Compare(parsed, dup, 1);
    (void)extracted_name;
    (void)optional_is_null;
    (void)meta_exists;
    (void)equal;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(dup);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}