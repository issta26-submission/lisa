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
//<ID> 578
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{ \"person\": { \"name\": \"tester\", \"id\": 123 }, \"active\": true }";
    size_t raw_len = strlen(raw_json) + 1;
    char *mutable_json = (char *)cJSON_malloc(raw_len);
    memset(mutable_json, 0, raw_len);
    strcpy(mutable_json, raw_json);
    cJSON_Minify(mutable_json);
    cJSON *root = cJSON_Parse(mutable_json);

    // step 2: Configure
    cJSON *person = cJSON_GetObjectItem(root, "person");
    cJSON *name_item = cJSON_GetObjectItem(person, "name");
    cJSON *id_item = cJSON_GetObjectItem(person, "id");
    cJSON *person_copy = cJSON_Duplicate(person, 1);
    cJSON_AddItemToObject(root, "person_copy", person_copy);

    // step 3: Operate
    const char *extracted_name = cJSON_GetStringValue(name_item);
    cJSON_bool name_invalid = cJSON_IsInvalid(name_item);
    cJSON_bool same_copy = cJSON_Compare(person, cJSON_GetObjectItem(root, "person_copy"), 1);
    cJSON_AddStringToObject(root, "extracted_name", extracted_name);
    cJSON_AddBoolToObject(root, "name_was_invalid", name_invalid);
    cJSON_AddBoolToObject(root, "person_equals_copy", same_copy);

    // step 4: Validate & Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    size_t scratch_len = printed_len + 32;
    void *scratch = cJSON_malloc(scratch_len);
    memset(scratch, 0, scratch_len);
    if (printed) strcpy((char *)scratch, printed);
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_free(mutable_json);
    // API sequence test completed successfully
    return 66;
}