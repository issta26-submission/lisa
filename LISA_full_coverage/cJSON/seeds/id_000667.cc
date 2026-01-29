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
//<ID> 667
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *obj = cJSON_CreateObject();
    cJSON *flag_true = cJSON_CreateTrue();
    cJSON *num = cJSON_CreateNumber(123.0);
    cJSON_AddItemToObject(root, "obj", obj);
    cJSON_AddItemToObject(obj, "flag", flag_true);
    cJSON_AddItemToObject(obj, "num", num);
    cJSON *nested = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "nested", nested);

    // step 2: Configure
    cJSON *dup_obj = cJSON_Duplicate(obj, 1);
    cJSON_AddItemToObject(nested, "dup_obj", dup_obj);

    // step 3: Operate
    cJSON *detached_from_root = cJSON_DetachItemFromObject(root, "obj");
    cJSON *detached_from_nested = cJSON_DetachItemFromObjectCaseSensitive(nested, "dup_obj");
    cJSON_AddItemToObject(root, "reattached", detached_from_nested);
    cJSON_AddItemToObject(nested, "reattached2", detached_from_root);

    // step 4: Validate & Cleanup
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    cJSON_PrintPreallocated(root, buffer, (int)sizeof(buffer), 0);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}