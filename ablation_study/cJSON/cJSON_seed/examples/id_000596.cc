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
//<ID> 596
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    cJSON *person = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("tester");
    cJSON_AddItemToObject(person, "name", name);
    cJSON_AddItemToObject(root, "person", person);

    // step 2: Configure
    cJSON *status_item = cJSON_CreateString("active");
    cJSON_AddItemToObject(root, "status", status_item);
    cJSON *orphan_null = cJSON_CreateNull();
    cJSON_bool orphan_is_invalid = cJSON_IsInvalid(orphan_null);

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON_bool has_person = cJSON_HasObjectItem(parsed, "person");
    cJSON *parsed_person = cJSON_GetObjectItem(parsed, "person");
    cJSON *parsed_name = cJSON_GetObjectItem(parsed_person, "name");
    const char *extracted_name = cJSON_GetStringValue(parsed_name);
    cJSON_bool orphan_is_invalid_after = cJSON_IsInvalid(orphan_null);

    // step 4: Cleanup
    char status[256];
    memset(status, 0, sizeof(status));
    sprintf(status, "ver=%s;has_person=%d;name=%s;invalid_orig=%d;invalid_after=%d",
            version ? version : "null",
            (int)has_person,
            extracted_name ? extracted_name : "null",
            (int)orphan_is_invalid,
            (int)orphan_is_invalid_after);
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    cJSON_Delete(orphan_null);
    // API sequence test completed successfully
    return 66;
}