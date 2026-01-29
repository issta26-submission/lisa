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
//<ID> 577
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{ \"person\": { \"name\": \"Alice\", \"age\": 30 }, \"status\": \"ok\" }";
    size_t raw_len = strlen(raw_json) + 1;
    char *mutable_json = (char *)cJSON_malloc(raw_len);
    memset(mutable_json, 0, raw_len);
    strcpy(mutable_json, raw_json);
    cJSON_Minify(mutable_json);
    cJSON *root = cJSON_Parse(mutable_json);
    cJSON *person = cJSON_GetObjectItem(root, "person");
    cJSON *name_item = cJSON_GetObjectItem(person, "name");

    // step 2: Configure
    char *old_name = cJSON_SetValuestring(name_item, "Alice Smith");
    cJSON_AddStringToObject(root, "greeting", "Hello, Alice");
    cJSON *compare_target = cJSON_CreateString("Alice Smith");

    // step 3: Operate
    char *extracted_name = cJSON_GetStringValue(name_item);
    cJSON_bool name_equals = cJSON_Compare(name_item, compare_target, 1);
    cJSON_bool name_is_invalid = cJSON_IsInvalid(name_item);
    char *printed = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    char *status_str = cJSON_GetStringValue(cJSON_GetObjectItem(root, "status"));
    cJSON *person_dup = cJSON_Duplicate(person, 1);
    cJSON_bool person_matches_dup = cJSON_Compare(person, person_dup, 1);
    (void)extracted_name;
    (void)name_equals;
    (void)name_is_invalid;
    (void)status_str;
    (void)person_matches_dup;
    (void)old_name; /* do not free this pointer separately; it is owned by name_item */
    cJSON_free(printed);
    cJSON_Delete(compare_target);
    cJSON_Delete(person_dup);
    cJSON_Delete(root);
    cJSON_free(mutable_json);
    // API sequence test completed successfully
    return 66;
}