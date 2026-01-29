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
//<ID> 737
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *src_json = "{\"id\":42,\"tags\":[\"one\",\"two\"]}";
    cJSON *orig = cJSON_Parse(src_json);
    cJSON *orig_tags = cJSON_GetObjectItem(orig, "tags");
    cJSON *first_orig_tag = cJSON_GetArrayItem(orig_tags, 0);
    const char *first_tag_str = cJSON_GetStringValue(first_orig_tag);
    cJSON *root = cJSON_CreateObject();
    cJSON *new_array = cJSON_CreateArray();
    cJSON *ref = cJSON_CreateStringReference(first_tag_str);
    cJSON *ref_manual = cJSON_CreateStringReference("manual");

    // step 2: Configure
    cJSON_AddItemToArray(new_array, ref);
    cJSON_AddItemToArray(new_array, ref_manual);
    cJSON_AddItemToObject(root, "merged_tags", new_array);
    cJSON_AddItemToObject(root, "source_id", cJSON_CreateNumber(42.0));

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *reparsed = cJSON_Parse(buffer);
    cJSON *got_merged = cJSON_GetObjectItem(reparsed, "merged_tags");
    cJSON *first = cJSON_GetArrayItem(got_merged, 0);
    const char *got_str = cJSON_GetStringValue(first);
    cJSON_bool is_array = cJSON_IsArray(got_merged);
    double id = cJSON_GetNumberValue(cJSON_GetObjectItem(reparsed, "source_id"));
    (void)got_str;
    (void)is_array;
    (void)id;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(reparsed);
    cJSON_Delete(root);
    cJSON_Delete(orig);
    // API sequence test completed successfully
    return 66;
}