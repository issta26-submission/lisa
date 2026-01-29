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
//<ID> 1411
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *data = cJSON_AddObjectToObject(root, "data");
    cJSON *nums = cJSON_CreateArray();
    cJSON_AddItemToObject(data, "nums", nums);

    // step 2: Configure
    cJSON_AddNumberToObject(data, "pi", 3.14159265);
    cJSON_AddItemToArray(nums, cJSON_CreateNumber(10.0));
    cJSON_AddItemToArray(nums, cJSON_CreateNumber(20.0));
    cJSON_AddItemToArray(nums, cJSON_CreateNumber(30.0));

    // step 3: Operate and Validate
    cJSON *pi_item = cJSON_GetObjectItemCaseSensitive(data, "pi");
    double pi_val = cJSON_GetNumberValue(pi_item);
    char *unformatted = cJSON_PrintUnformatted(root);
    char *scratch = (char *)cJSON_malloc((size_t)64);
    memset(scratch, 0, (size_t)64);
    scratch[0] = unformatted[0];
    scratch[1] = (char)('0' + ((int)pi_val % 10));
    scratch[2] = (char)('0' + (((int)pi_val / 10) % 10));
    scratch[3] = (char)('0' + (((int)pi_val / 100) % 10));

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_free(unformatted);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}