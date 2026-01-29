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
//<ID> 753
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *json_text = "{\"person\":{\"name\":\"alice\",\"age\":30},\"notes\":\"initial\"}";
    cJSON *root = cJSON_Parse(json_text);

    // step 2: Configure
    cJSON *person = cJSON_GetObjectItem(root, "person");
    cJSON *name = cJSON_GetObjectItem(person, "name");
    char *old_name = cJSON_SetValuestring(name, "alice_updated");
    (void)old_name;
    cJSON *raw = cJSON_CreateRaw("{\"rawkey\":123}");
    cJSON_AddItemToObject(root, "rawdata", raw);
    cJSON *detached_notes = cJSON_DetachItemFromObject(root, "notes");
    cJSON_AddItemToObject(person, "notes_detached", detached_notes);
    cJSON_DeleteItemFromObjectCaseSensitive(person, "age");

    // step 3: Operate
    char *printed = cJSON_PrintBuffered(root, 256, 1);

    // step 4: Validate & Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}