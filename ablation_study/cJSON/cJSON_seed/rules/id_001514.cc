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
//<ID> 1514
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *parsed = NULL;
    cJSON *person = NULL;
    cJSON *name_item = NULL;
    cJSON *note_item = NULL;
    cJSON *str_ref = NULL;
    char *name_str = NULL;
    char *note_str = NULL;
    char *ref_str = NULL;
    const char *parse_end = NULL;
    char json[] = "{\"person\":{\"name\":\"Bob\",\"age\":30},\"note\":\"hello\"}";

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithOpts(json, &parse_end, 1);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);
    str_ref = cJSON_CreateStringReference("external-ref");
    cJSON_AddItemToObject(root, "ref", str_ref);

    // step 3: Operate / Validate
    person = cJSON_GetObjectItem(parsed, "person");
    name_item = cJSON_GetObjectItem(person, "name");
    note_item = cJSON_GetObjectItem(parsed, "note");
    name_str = cJSON_GetStringValue(name_item);
    note_str = cJSON_GetStringValue(note_item);
    ref_str = cJSON_GetStringValue(str_ref);
    (void)parse_end;
    (void)name_str;
    (void)note_str;
    (void)ref_str;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}