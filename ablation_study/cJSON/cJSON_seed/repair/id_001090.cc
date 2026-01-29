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
//<ID> 1090
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json[] = "{\"name\":\"initial\",\"count\":1}";
    cJSON *parsed = cJSON_ParseWithLength(json, sizeof(json) - 1);
    cJSON *root = cJSON_CreateObject();
    cJSON_AddItemReferenceToObject(root, "external", parsed);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "added_number", 3.1415);
    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(parsed, "name");
    cJSON_SetValuestring(name_item, "modified_name");

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    size_t scratch_size = 64;
    void *scratch = cJSON_malloc(scratch_size);
    memset(scratch, 0, scratch_size);
    ((char *)scratch)[0] = out[0];
    cJSON_free(out);

    // step 4: Cleanup
    cJSON *detached = cJSON_DetachItemViaPointer(root, parsed);
    cJSON_Delete(detached);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}