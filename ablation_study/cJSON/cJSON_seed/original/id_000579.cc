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
//<ID> 579
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = " { \"name\" : \"tester\", \"id\" : 123, \"active\" : true } ";
    size_t raw_len = strlen(raw_json) + 1;
    char *mutable_json = (char *)cJSON_malloc(raw_len);
    memset(mutable_json, 0, raw_len);
    strcpy(mutable_json, raw_json);
    cJSON_Minify(mutable_json);
    cJSON *root = cJSON_Parse(mutable_json);

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItem(root, "name");
    char *name_value = (char *)cJSON_GetStringValue(name_item);
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON *dup_name_item = cJSON_GetObjectItem(dup, "name");
    cJSON_bool equal_before = cJSON_Compare(root, dup, 1);
    cJSON_bool name_is_invalid = cJSON_IsInvalid(name_item);

    // step 3: Operate
    char *old_name = cJSON_SetValuestring(dup_name_item, "tester_modified");
    char *printed = cJSON_Print(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    size_t scratch_len = printed_len + 32;
    char *scratch = (char *)cJSON_malloc(scratch_len);
    memset(scratch, 0, scratch_len);
    strcpy(scratch, printed);
    const char *extracted_dup_name = cJSON_GetStringValue(cJSON_GetObjectItem(dup, "name"));

    // step 4: Validate & Cleanup
    (void)name_value;
    (void)equal_before;
    (void)name_is_invalid;
    (void)old_name;
    (void)extracted_dup_name;
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(dup);
    cJSON_Delete(root);
    cJSON_free(mutable_json);
    // API sequence test completed successfully
    return 66;
}