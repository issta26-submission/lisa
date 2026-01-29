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
//<ID> 573
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{ \"person\": { \"name\": \"Alice\", \"age\": 30 }, \"note\": \"greeting\" }";
    size_t raw_len = strlen(raw_json) + 1;
    char *mutable_json = (char *)cJSON_malloc(raw_len);
    memset(mutable_json, 0, raw_len);
    strcpy(mutable_json, raw_json);
    cJSON_Minify(mutable_json);
    cJSON *root = cJSON_Parse(mutable_json);

    // step 2: Configure
    cJSON *person = cJSON_GetObjectItem(root, "person");
    cJSON *name_item = cJSON_GetObjectItem(person, "name");
    cJSON *age_item = cJSON_GetObjectItem(person, "age");
    cJSON *note_item = cJSON_GetObjectItem(root, "note");
    cJSON *person_copy = cJSON_Duplicate(person, 1);
    cJSON_AddItemToObject(root, "person_copy", person_copy);
    cJSON *alias_item = cJSON_CreateString("A.lice");
    cJSON_AddItemToObject(person_copy, "alias", alias_item);

    // step 3: Operate
    const char *name_str = cJSON_GetStringValue(name_item);
    const char *note_str = cJSON_GetStringValue(note_item);
    cJSON_bool name_is_invalid = cJSON_IsInvalid(name_item);
    cJSON_bool names_equal = cJSON_Compare(name_item, cJSON_GetObjectItem(person_copy, "name"), 1);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = printed ? strlen(printed) : 0;
    char *scratch = (char *)cJSON_malloc(printed_len + 32);
    memset(scratch, 0, printed_len + 32);
    if (printed) strcpy(scratch, printed);

    // step 4: Validate & Cleanup
    const char *extracted_alias = cJSON_GetStringValue(cJSON_GetObjectItem(person_copy, "alias"));
    (void)name_str;
    (void)note_str;
    (void)name_is_invalid;
    (void)names_equal;
    (void)extracted_alias;
    cJSON_free(printed);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_free(mutable_json);
    // API sequence test completed successfully
    return 66;
}