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
//<ID> 526
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("initial_name");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *status = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "status", status);
    cJSON *details = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "details", details);
    cJSON *note = cJSON_CreateString("initial_note");
    cJSON_AddItemToObject(details, "note", note);

    // step 2: Configure
    cJSON *replacement_name = cJSON_CreateString("replaced_name");
    cJSON_bool replaced_ok = cJSON_ReplaceItemViaPointer(root, name, replacement_name);
    (void)replaced_ok;

    // step 3: Operate
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(root, "name");
    char *extracted = cJSON_GetStringValue(name_item);
    cJSON_AddItemToObject(root, "extracted_name", cJSON_CreateString(extracted));
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON *status_item = cJSON_GetObjectItemCaseSensitive(root, "status");
    cJSON_bool status_was_false = cJSON_IsFalse(status_item);
    cJSON_AddItemToObject(root, "status_was_false", cJSON_CreateBool(status_was_false));

    // step 4: Validate and Cleanup
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}