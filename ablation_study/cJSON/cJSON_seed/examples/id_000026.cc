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
//<ID> 26
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
    cJSON_bool added_root_config = cJSON_AddItemToObject(root, "config", config);

    // step 2: Configure
    char *name_buf = (char*)cJSON_malloc(16);
    memset(name_buf, 0, 16);
    name_buf[0] = 't';
    name_buf[1] = 'e';
    name_buf[2] = 's';
    name_buf[3] = 't';
    name_buf[4] = '\0';
    cJSON *name_item = cJSON_CreateString(name_buf);
    cJSON_bool added_name = cJSON_AddItemToObject(config, "name", name_item);
    cJSON *pi_item = cJSON_CreateNumber(3.14159);
    cJSON_bool added_pi = cJSON_AddItemToObject(config, "pi", pi_item);
    cJSON *nullable_item = cJSON_CreateNull();
    cJSON_bool added_nullable = cJSON_AddItemToObject(config, "nullable", nullable_item);

    // step 3: Operate & Validate
    cJSON *fetched_pi = cJSON_GetObjectItem(config, "pi");
    double pi_value = cJSON_GetNumberValue(fetched_pi);
    (void)pi_value;
    (void)added_root_config;
    (void)added_name;
    (void)added_pi;
    (void)added_nullable;

    // step 4: Cleanup
    cJSON_free(name_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}