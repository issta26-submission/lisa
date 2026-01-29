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
//<ID> 1527
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *container = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "container", container);

    // step 2: Configure
    cJSON_AddStringToObject(container, "name", "initial");
    cJSON *replacement = cJSON_CreateString("replaced");
    cJSON_ReplaceItemInObject(container, "name", replacement);
    const char *version = cJSON_Version();

    // step 3: Operate and Validate
    cJSON *got_container = cJSON_GetObjectItemCaseSensitive(root, "container");
    cJSON *got_name = cJSON_GetObjectItemCaseSensitive(got_container, "name");
    const char *name_val = cJSON_GetStringValue(got_name);
    char *out = cJSON_PrintUnformatted(root);
    size_t out_len = out ? (size_t)strlen(out) : (size_t)0;
    char *scratch = (char *)cJSON_malloc(out_len + 8);
    memset(scratch, 0, out_len + 8);
    scratch[0] = version ? version[0] : 'v';
    scratch[1] = name_val ? name_val[0] : 'n';
    scratch[2] = out_len ? out[0] : 'x';

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}