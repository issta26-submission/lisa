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
//<ID> 1975
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *name = cJSON_CreateString("Alice");
    cJSON_AddItemToObject(root, "name", name);
    cJSON *name_ref = cJSON_CreateStringReference("Alice");
    cJSON_AddItemToObject(root, "name_ref", name_ref);
    cJSON *status_old = cJSON_CreateString("old");
    cJSON_AddItemToObject(root, "status", status_old);

    // step 2: Configure
    cJSON *status_new = cJSON_CreateString("updated");
    cJSON_ReplaceItemViaPointer(root, status_old, status_new);
    cJSON *note = cJSON_CreateString("configured");
    cJSON_AddItemToObject(root, "note", note);

    // step 3: Operate and Validate
    char *unformatted = cJSON_PrintUnformatted(root);
    int buflen = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_PrintPreallocated(root, buffer, buflen, 1);
    cJSON_free(unformatted);
    cJSON_free(buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}