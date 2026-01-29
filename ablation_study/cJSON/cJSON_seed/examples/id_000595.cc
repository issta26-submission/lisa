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
//<ID> 595
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
    cJSON *name_item = cJSON_CreateString("Eve");
    cJSON_AddItemToObject(person, "name", name_item);
    cJSON *age_item = cJSON_CreateNumber(29.0);
    cJSON_AddItemToObject(person, "age", age_item);
    cJSON_AddItemToObject(root, "person", person);

    // step 2: Configure
    cJSON *status_item = cJSON_CreateString("active");
    cJSON_AddItemToObject(root, "status", status_item);
    cJSON *note_item = cJSON_CreateString("none");
    cJSON_bool add_note_ok = cJSON_AddItemToObject(root, "note", note_item);

    // step 3: Operate & Validate
    cJSON_bool has_person = cJSON_HasObjectItem(root, "person");
    cJSON *got_person = cJSON_GetObjectItem(root, "person");
    cJSON *got_name = cJSON_GetObjectItem(got_person, "name");
    const char *name_value = cJSON_GetStringValue(got_name);
    cJSON_bool name_invalid = cJSON_IsInvalid(got_name);
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_person = cJSON_GetObjectItem(parsed, "person");
    cJSON *parsed_name = cJSON_GetObjectItem(parsed_person, "name");
    const char *parsed_name_value = cJSON_GetStringValue(parsed_name);

    // step 4: Cleanup
    char status_buf[256];
    memset(status_buf, 0, sizeof(status_buf));
    sprintf(status_buf,
            "ver=%s;has_person=%d;name=%s;name_invalid=%d;add_note_ok=%d;parsed_name=%s",
            version ? version : "null",
            (int)has_person,
            name_value ? name_value : "null",
            (int)name_invalid,
            (int)add_note_ok,
            parsed_name_value ? parsed_name_value : "null");
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}