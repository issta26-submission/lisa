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
//<ID> 1009
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddStringToObject(root, "name", "original");
    cJSON_AddStringToObject(meta, "note", "initial note");

    // step 2: Configure
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    cJSON_SetValuestring(name_item, "updated");
    cJSON *note_item = cJSON_GetObjectItemCaseSensitive(meta, "note");
    cJSON_SetValuestring(note_item, "updated note");

    // step 3: Operate and Validate
    char *flat = cJSON_PrintUnformatted(root);
    cJSON_free(flat);
    char *pretty = cJSON_Print(root);
    cJSON_free(pretty);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}