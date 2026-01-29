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
//<ID> 1232
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *items = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", items);
    cJSON *num = cJSON_CreateNumber(42.5);
    cJSON_AddItemToArray(items, num);
    cJSON *sref = cJSON_CreateStringReference("example_ref");
    cJSON_AddItemToArray(items, sref);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure
    double base_val = cJSON_GetNumberValue(num);
    cJSON_bool is_bool = cJSON_IsBool(flag);
    cJSON_bool is_false = cJSON_IsFalse(flag);
    double computed_val = base_val + (double)is_bool + (double)is_false;
    cJSON *computed_num = cJSON_CreateNumber(computed_val);
    cJSON_AddItemToObject(root, "computed", computed_num);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];
    cJSON_free(out);
    cJSON_free(scratch);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}