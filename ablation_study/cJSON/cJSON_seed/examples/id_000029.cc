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
//<ID> 29
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
    cJSON *pi_item = cJSON_CreateNumber(3.141592653589793);
    cJSON_AddItemToObject(config, "pi", pi_item);
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToObject(config, "nullable", null_item);
    char *name_buf = (char*)cJSON_malloc(16);
    name_buf[0] = 'a'; name_buf[1] = 'l'; name_buf[2] = 'i'; name_buf[3] = 'a'; name_buf[4] = 's'; name_buf[5] = '\0';
    cJSON *name_ref = cJSON_CreateStringReference((const char*)name_buf);
    cJSON_AddItemToObject(config, "name", name_ref);

    // step 3: Operate & Validate
    cJSON *fetched_pi = cJSON_GetObjectItem(config, "pi");
    double pi_val = cJSON_GetNumberValue(fetched_pi);
    cJSON *double_pi = cJSON_CreateNumber(pi_val * 2.0);
    cJSON_AddItemToObject(config, "double_pi", double_pi);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(name_buf);

    // API sequence test completed successfully
    return 66;
}