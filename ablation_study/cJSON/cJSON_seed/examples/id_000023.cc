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
//<ID> 23
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "child", child);

    // step 2: Configure
    cJSON *num = cJSON_CreateNumber(42.5);
    cJSON_AddItemToObject(child, "pi_like", num);
    char *dyn = (char *)cJSON_malloc(16);
    memset(dyn, 0, 16);
    dyn[0] = 'H';
    dyn[1] = 'i';
    dyn[2] = '!';
    dyn[3] = '\0';
    cJSON *greeting = cJSON_CreateString(dyn);
    cJSON_AddItemToObject(child, "greeting", greeting);
    cJSON *nul = cJSON_CreateNull();
    cJSON_AddItemToObject(root, "nullable", nul);

    // step 3: Operate & Validate
    cJSON *fetched_num = cJSON_GetObjectItem(child, "pi_like");
    double val = cJSON_GetNumberValue(fetched_num);
    double adjusted_val = val + 1.5;
    cJSON *adjusted = cJSON_CreateNumber(adjusted_val);
    cJSON_AddItemToObject(root, "adjusted", adjusted);

    // step 4: Cleanup
    cJSON_free(dyn);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}