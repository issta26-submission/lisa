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
//<ID> 572
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{ \"person\": { \"name\": \"Alice\", \"age\": 30 }, \"active\": true }";
    size_t raw_len = strlen(raw_json) + 1;
    char *mutable_json = (char *)cJSON_malloc(raw_len);
    memset(mutable_json, 0, raw_len);
    strcpy(mutable_json, raw_json);
    cJSON_Minify(mutable_json);
    cJSON *root = cJSON_Parse(mutable_json);

    // step 2: Configure
    cJSON *person = cJSON_GetObjectItem(root, "person");
    cJSON *name_item = cJSON_GetObjectItem(person, "name");
    const char *name_str = cJSON_GetStringValue(name_item);
    cJSON *dup_person = cJSON_Duplicate(person, 1);
    cJSON_bool are_equal = cJSON_Compare(person, dup_person, 1);
    cJSON_bool is_invalid = cJSON_IsInvalid(dup_person);
    cJSON_AddItemToObject(root, "person_copy", dup_person);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    size_t scratch_len = printed_len + 32;
    void *scratch = cJSON_malloc(scratch_len);
    memset(scratch, 0, scratch_len);
    strcpy((char *)scratch, printed);

    // step 4: Validate & Cleanup
    const char *copied_name = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetObjectItem(root, "person_copy"), "name"));
    (void)name_str;
    (void)copied_name;
    (void)are_equal;
    (void)is_invalid;
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_free(mutable_json);
    // API sequence test completed successfully
    return 66;
}