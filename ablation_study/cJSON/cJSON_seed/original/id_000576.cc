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
//<ID> 576
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{ \"user\": { \"name\": \"alice\", \"age\": 30 }, \"active\": true }";
    size_t raw_len = strlen(raw_json) + 1;
    char *mutable_json = (char *)cJSON_malloc(raw_len);
    memset(mutable_json, 0, raw_len);
    strcpy(mutable_json, raw_json);
    cJSON_Minify(mutable_json);
    cJSON *root = cJSON_Parse(mutable_json);

    // step 2: Configure
    cJSON *user = cJSON_GetObjectItem(root, "user");
    cJSON *name_item = cJSON_GetObjectItem(user, "name");
    const char *name_str = cJSON_GetStringValue(name_item);
    cJSON *temp_name = cJSON_CreateString("alice");
    cJSON_bool name_equal = cJSON_Compare(name_item, temp_name, 1);
    cJSON_AddBoolToObject(root, "name_equal", name_equal);
    cJSON *missing = cJSON_GetObjectItem(root, "missing");
    cJSON_bool missing_invalid = cJSON_IsInvalid(missing);
    cJSON_AddBoolToObject(root, "missing_invalid", missing_invalid);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    size_t buf_len = printed_len + 32;
    char *scratch = (char *)cJSON_malloc(buf_len);
    memset(scratch, 0, buf_len);
    if (printed) strcpy(scratch, printed);
    cJSON_free(printed);
    cJSON_Delete(temp_name);
    cJSON *user_dup = cJSON_Duplicate(user, 1);
    cJSON_bool users_same = cJSON_Compare(user, user_dup, 1);
    cJSON_AddBoolToObject(root, "users_same", users_same);

    // step 4: Validate & Cleanup
    const char *extracted_name = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetObjectItem(root, "user"), "name"));
    (void)name_str;
    (void)extracted_name;
    cJSON_free(scratch);
    cJSON_Delete(user_dup);
    cJSON_Delete(root);
    cJSON_free(mutable_json);
    // API sequence test completed successfully
    return 66;
}