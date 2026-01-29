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
//<ID> 27
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *pi = cJSON_CreateNumber(3.141592653589793);
    cJSON *label = cJSON_CreateString("pi_value");
    cJSON *nothing = cJSON_CreateNull();

    // step 2: Configure
    cJSON_AddItemToObject(root, "pi", pi);
    cJSON_AddItemToObject(root, "label", label);
    cJSON_AddItemToObject(root, "nothing", nothing);

    // step 3: Operate & Validate
    cJSON *fetched_pi = cJSON_GetObjectItem(root, "pi");
    double pi_val = cJSON_GetNumberValue(fetched_pi);
    void *buffer = cJSON_malloc(64);
    memset(buffer, 0, 64);
    int written = snprintf((char *)buffer, 64, "%.6f", pi_val);
    (void)written;
    cJSON *pi_double = cJSON_CreateNumber(pi_val * 2.0);
    cJSON_AddItemToObject(root, "pi_times_two", pi_double);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}