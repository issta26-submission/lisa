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
//<ID> 194
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char json_text[] = "{\"id\":123,\"name\":\"example\"}";
    const char *parse_end = NULL;
    size_t json_len = sizeof(json_text) - 1;
    cJSON *root = cJSON_CreateObject();
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *name_item = NULL;
    cJSON *note_item = NULL;
    char buffer[64];
    memset(buffer, 0, sizeof(buffer));
    const char *name_str = NULL;
    char *serialized = NULL;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "data", parsed);
    cJSON_AddStringToObject(root, "note", "created_via_api");

    // step 3: Operate / Validate
    name_item = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    note_item = cJSON_GetObjectItemCaseSensitive(root, "note");
    name_str = cJSON_GetStringValue(name_item);
    /* copy known-length literal "example" into buffer to demonstrate data flow */
    memcpy(buffer, name_str, sizeof("example") - 1);
    buffer[sizeof("example") - 1] = '\0';
    serialized = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(serialized);
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)parse_end;
    (void)note_item;
    (void)buffer;
    return 66;
}