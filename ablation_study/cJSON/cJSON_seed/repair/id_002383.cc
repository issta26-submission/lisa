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
//<ID> 2383
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
    cJSON *pi_item = cJSON_CreateNumber(3.14159);

    // step 2: Configure
    cJSON_AddItemToObject(config, "pi", pi_item);
    cJSON_AddItemToObject(root, "config", config);
    cJSON *enabled_item = cJSON_AddTrueToObject(root, "enabled");

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    cJSON_bool has_enabled = cJSON_HasObjectItem(root, "enabled");
    cJSON *config_found = cJSON_GetObjectItem(root, "config");
    cJSON *pi_found = cJSON_GetObjectItem(config_found, "pi");
    double pi_val = cJSON_GetNumberValue(pi_found);
    buffer[0] = printed[0];
    buffer[1] = (char)(int)pi_val;
    buffer[2] = (char)(int)has_enabled;

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}