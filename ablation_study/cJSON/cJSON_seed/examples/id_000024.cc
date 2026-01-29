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
//<ID> 24
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
    cJSON *pi_item = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToObject(config, "pi", pi_item);
    cJSON *nullable = cJSON_CreateNull();
    cJSON_AddItemToObject(config, "nullable", nullable);
    double *computed = (double *)cJSON_malloc(sizeof(double));
    memset(computed, 0, sizeof(double));

    // step 3: Operate & Validate
    cJSON *fetched_pi = cJSON_GetObjectItem(config, "pi");
    double pi_value = cJSON_GetNumberValue(fetched_pi);
    *computed = pi_value + 1.0;
    (void)pi_value;
    (void)fetched_pi;

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(computed);
    // API sequence test completed successfully
    return 66;
}