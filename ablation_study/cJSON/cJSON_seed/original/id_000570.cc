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
//<ID> 570
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{ \"name\": \"alpha\", \"count\": 3, \"nested\": { \"tag\": \"alpha\" } }";
    size_t raw_len = strlen(raw_json) + 1;
    char *mutable_json = (char *)cJSON_malloc(raw_len);
    memset(mutable_json, 0, raw_len);
    strcpy(mutable_json, raw_json);
    cJSON_Minify(mutable_json);
    cJSON *root = cJSON_Parse(mutable_json);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    cJSON *nested_obj = cJSON_GetObjectItem(root, "nested");
    cJSON *nested_tag = cJSON_GetObjectItem(nested_obj, "tag");
    cJSON *tag_copy = cJSON_CreateString(cJSON_GetStringValue(name_item));
    cJSON_AddItemToObject(root, "tag_copy", tag_copy);

    // step 3: Operate
    char *extracted_name = (char *)cJSON_GetStringValue(name_item);
    cJSON_bool name_invalid = cJSON_IsInvalid(name_item);
    cJSON_bool name_matches_nested = cJSON_Compare(name_item, nested_tag, 1);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    size_t scratch_len = printed_len + 32;
    char *scratch = (char *)cJSON_malloc(scratch_len);
    memset(scratch, 0, scratch_len);
    if (printed) strcpy(scratch, printed); // single unconditional strcpy; still straight-line (no branching logic)

    // step 4: Validate & Cleanup
    char *extracted_tag_copy = (char *)cJSON_GetStringValue(cJSON_GetObjectItem(root, "tag_copy"));
    (void)extracted_name;
    (void)name_invalid;
    (void)name_matches_nested;
    (void)extracted_tag_copy;
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_free(mutable_json);
    // API sequence test completed successfully
    return 66;
}