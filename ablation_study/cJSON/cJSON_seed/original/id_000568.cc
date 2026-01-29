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
//<ID> 568
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{ \"person\": { \"name\": \"original\", \"age\": 30 }, \"tags\": [\"one\"] }";
    size_t raw_len = strlen(raw_json) + 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(raw_json, raw_len, &parse_end, 1);
    cJSON *person = cJSON_GetObjectItem(root, "person");
    cJSON *name_item = cJSON_GetObjectItem(person, "name");
    cJSON *tags = cJSON_GetObjectItem(root, "tags");

    // step 2: Configure
    char *new_name_ptr = cJSON_SetValuestring(name_item, "modified");
    cJSON *tag_ref = cJSON_CreateStringReference("referenced");
    cJSON_bool add_ref_result = cJSON_AddItemReferenceToArray(tags, tag_ref);

    // step 3: Operate
    cJSON_bool has_person = cJSON_HasObjectItem(root, "person");
    cJSON *present_flag = cJSON_CreateNumber((double)has_person);
    cJSON_AddItemToObject(root, "has_person", present_flag);
    char *printed = cJSON_PrintBuffered(root, 64, 1);

    // step 4: Validate & Cleanup
    const char *extracted_name = cJSON_GetStringValue(cJSON_GetObjectItem(cJSON_GetObjectItem(root, "person"), "name"));
    (void)new_name_ptr;
    (void)add_ref_result;
    (void)extracted_name;
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}