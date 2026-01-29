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
//<ID> 661
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *extra = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "extra", extra);
    cJSON_AddItemToArray(items, cJSON_CreateString("alpha"));
    cJSON_AddItemToArray(items, cJSON_CreateString("beta"));
    cJSON_AddItemToArray(extra, cJSON_CreateNumber(42.0));

    // step 2: Configure
    int bufsize = 1024;
    char *buffer = (char *)cJSON_malloc((size_t)bufsize);
    memset(buffer, 0, bufsize);
    cJSON_bool printed = cJSON_PrintPreallocated(root, buffer, bufsize, 0);
    (void)printed;

    // step 3: Operate and Validate
    cJSON *first = cJSON_GetArrayItem(items, 0);
    cJSON_bool first_is_string = cJSON_IsString(first);
    cJSON_AddBoolToObject(root, "first_is_string", first_is_string);
    cJSON *items_copy = cJSON_Duplicate(items, 1);
    cJSON_AddItemToObject(root, "items_copy", items_copy);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}