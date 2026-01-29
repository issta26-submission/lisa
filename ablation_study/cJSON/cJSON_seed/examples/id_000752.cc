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
//<ID> 752
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *raw_json = "{\"person\":{\"name\":\"Eve\",\"age\":29},\"active\":null}";
    cJSON *parsed = cJSON_Parse(raw_json);
    cJSON *root = cJSON_CreateObject();
    cJSON *person = cJSON_CreateObject();
    cJSON *name_item = cJSON_CreateString("Alice");
    cJSON *age_item = cJSON_CreateNumber(42.0);
    cJSON *maybe_null = cJSON_CreateNull();

    // step 2: Configure
    cJSON_bool add_name_ok = cJSON_AddItemToObject(person, "name", name_item);
    cJSON_bool add_age_ok = cJSON_AddItemToObject(person, "age", age_item);
    cJSON_bool add_person_ok = cJSON_AddItemToObject(root, "person", person);
    cJSON_bool add_maybe_ok = cJSON_AddItemToObject(root, "maybe", maybe_null);
    cJSON_bool has_person_in_parsed = cJSON_HasObjectItem(parsed, "person");
    cJSON *parsed_active = cJSON_GetObjectItem(parsed, "active");
    cJSON_bool parsed_active_is_null = cJSON_IsNull(parsed_active);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *reparsed = cJSON_Parse(buffer);
    cJSON *found_person = cJSON_GetObjectItem(reparsed, "person");
    cJSON *found_name = cJSON_GetObjectItem(found_person, "name");
    const char *extracted_name = cJSON_GetStringValue(found_name);
    double extracted_age = cJSON_GetNumberValue(cJSON_GetObjectItem(found_person, "age"));
    double validation_metric = extracted_age + (double)has_person_in_parsed + (double)parsed_active_is_null + (double)add_name_ok + (double)add_age_ok + (double)add_person_ok + (double)add_maybe_ok;
    (void)extracted_name;
    (void)validation_metric;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(reparsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}