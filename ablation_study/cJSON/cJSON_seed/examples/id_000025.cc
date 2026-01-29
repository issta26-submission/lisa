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
//<ID> 25
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
    double *pi_storage = (double*)cJSON_malloc(sizeof(double));
    *pi_storage = 3.141592653589793;
    cJSON *pi_item = cJSON_CreateNumber(*pi_storage);
    cJSON_AddItemToObject(config, "pi", pi_item);
    cJSON *null_entry = cJSON_CreateNull();
    cJSON_AddItemToObject(config, "nullable", null_entry);

    // step 2: Configure
    cJSON *version_item = cJSON_CreateNumber(1.618033988749895);
    cJSON_AddItemToObject(root, "version", version_item);
    cJSON *mult_item = cJSON_CreateNumber(2.0);
    cJSON_AddItemToObject(config, "multiplier", mult_item);

    // step 3: Operate & Validate
    cJSON *fetched_config = cJSON_GetObjectItem(root, "config");
    cJSON *fetched_pi = cJSON_GetObjectItem(fetched_config, "pi");
    double pi_value = cJSON_GetNumberValue(fetched_pi);
    cJSON *fetched_mult = cJSON_GetObjectItem(fetched_config, "multiplier");
    double mult_value = cJSON_GetNumberValue(fetched_mult);
    cJSON *computed = cJSON_CreateNumber(pi_value * mult_value);
    cJSON_AddItemToObject(root, "computed_pi_times_mult", computed);
    double computed_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "computed_pi_times_mult"));
    (void)pi_value;
    (void)mult_value;
    (void)computed_value;

    // step 4: Cleanup
    cJSON_free(pi_storage);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}