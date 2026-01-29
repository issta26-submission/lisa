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
//<ID> 2109
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *cfg = cJSON_AddObjectToObject(root, "config");
    cJSON *num_item = cJSON_CreateNumber(3.14);
    cJSON_AddItemToObject(cfg, "pi", num_item);
    cJSON *name_item = cJSON_CreateString("cJSON_fuzz");
    cJSON_AddItemToObject(cfg, "name", name_item);

    // step 2: Configure
    double newval = cJSON_SetNumberHelper(num_item, 2.718);

    // step 3: Operate and Validate
    cJSON *detached_name = cJSON_DetachItemFromObject(cfg, "name");
    const char *name_str = cJSON_GetStringValue(detached_name);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = name_str ? name_str[0] : '\0';
    buffer[1] = (char)('0' + (int)newval);
    buffer[2] = '\0';

    // step 4: Cleanup
    cJSON_Delete(detached_name);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}