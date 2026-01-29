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
//<ID> 160
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "config", config);

    // step 2: Configure
    cJSON *opt_null = cJSON_CreateNull();
    cJSON_AddItemToObject(config, "option", opt_null);
    void *mem = cJSON_malloc(16);
    char *dyn = (char *)mem;
    dyn[0] = 'h'; dyn[1] = 'e'; dyn[2] = 'l'; dyn[3] = 'l'; dyn[4] = 'o'; dyn[5] = '\0';
    cJSON *s_dyn = cJSON_CreateString(dyn);
    cJSON_AddItemToObject(root, "dynamic", s_dyn);

    // step 3: Operate and Validate
    cJSON *dup_string = cJSON_Duplicate(s_dyn, 1);
    cJSON_ReplaceItemViaPointer(config, opt_null, dup_string);
    cJSON *root_copy = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, root_copy, 1);
    cJSON_AddBoolToObject(root, "equal_copy", equal);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root_copy);
    cJSON_Delete(root);
    cJSON_free(mem);
    return 66; // API sequence test completed successfully
}