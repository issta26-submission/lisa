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
//<ID> 580
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
    cJSON *user = cJSON_GetObjectItem(root, "user");

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(user, "name");
    char *name_value = cJSON_GetStringValue(name_item);
    cJSON *dup_user = cJSON_Duplicate(user, 1);
    cJSON_AddItemToObject(root, "user_copy", dup_user);
    cJSON *age_item = cJSON_GetObjectItem(user, "age");

    // step 3: Operate
    char *printed = cJSON_PrintBuffered(root, 256, 1);
    int prelen = 512;
    char *prebuf = (char *)cJSON_malloc(prelen);
    memset(prebuf, 0, prelen);
    cJSON_PrintPreallocated(root, prebuf, prelen, 1);
    cJSON_DeleteItemFromObjectCaseSensitive(user, "age");
    char *printed_after = cJSON_PrintBuffered(root, 128, 1);

    // step 4: Validate & Cleanup
    char *extracted_name = cJSON_GetStringValue(cJSON_GetObjectItem(user, "name"));
    (void)name_value;
    (void)extracted_name;
    cJSON_free(printed);
    cJSON_free(prebuf);
    cJSON_free(printed_after);
    cJSON_Delete(root);
    cJSON_free(mutable_json);
    // API sequence test completed successfully
    return 66;
}