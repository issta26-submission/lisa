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
//<ID> 1513
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *parsed = NULL;
    const char *parse_end = NULL;
    cJSON *root = NULL;
    cJSON *imported = NULL;
    cJSON *person = NULL;
    cJSON *name_item = NULL;
    cJSON *name_ref = NULL;
    cJSON *note_item = NULL;
    const char *name_val = NULL;
    const char *note_val = NULL;
    char prebuf[256];
    memset(prebuf, 0, sizeof(prebuf));
    char json[] = "{ \"person\": { \"name\": \"Bob\", \"age\": 30 }, \"note\": \"hello\" }";

    // step 2: Setup / Configure
    parsed = cJSON_ParseWithOpts(json, &parse_end, 1);
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "imported", parsed);
    imported = cJSON_GetObjectItem(root, "imported");
    person = cJSON_GetObjectItem(imported, "person");
    name_item = cJSON_GetObjectItem(person, "name");
    note_item = cJSON_GetObjectItem(imported, "note");

    // step 3: Operate / Validate
    name_val = cJSON_GetStringValue(name_item);
    note_val = cJSON_GetStringValue(note_item);
    name_ref = cJSON_CreateStringReference(name_val);
    cJSON_AddItemToObject(root, "name_ref", name_ref);
    cJSON_PrintPreallocated(root, prebuf, (int)sizeof(prebuf), 0);
    (void)parse_end;
    (void)note_val;
    (void)prebuf;

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}