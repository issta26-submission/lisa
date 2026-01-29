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
//<ID> 1100
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON *values = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "values", values);
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON *flag = cJSON_AddFalseToObject(root, "active");

    // step 2: Configure
    cJSON *values_ref = cJSON_CreateArrayReference((const cJSON *)values);
    cJSON_AddItemToObject(meta, "values_ref", values_ref);
    cJSON *n3 = cJSON_CreateNumber(30.0);
    cJSON_AddItemToArray(values, n3);

    // step 3: Operate and Validate
    cJSON_bool is_bool = cJSON_IsBool(flag);
    cJSON_bool is_false = cJSON_IsFalse(flag);
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out ? out[0] : '\0';
    ((char *)scratch)[1] = (char)('0' + (int)is_bool);
    ((char *)scratch)[2] = (char)('0' + (int)is_false);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}