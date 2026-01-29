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
//<ID> 65
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *config = cJSON_AddObjectToObject(root, "config");
    cJSON *name = cJSON_CreateString("init");
    cJSON_AddItemToObject(config, "name", name);
    cJSON_AddNumberToObject(config, "count", 7.0);

    // step 2: Configure
    char *set_ret = cJSON_SetValuestring(name, "tester");
    cJSON_AddNumberToObject(root, "base", 5.0);

    // step 3: Operate and Validate
    cJSON_bool has_config = cJSON_HasObjectItem(root, "config");
    cJSON *count_item = cJSON_GetObjectItem(config, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    double validation = count_val + (double)has_config + 0.5;
    cJSON_AddNumberToObject(root, "validation", validation);
    double set_ok = (double)(set_ret != NULL);
    cJSON_AddNumberToObject(root, "set_ok", set_ok);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}