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
//<ID> 1163
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
    cJSON *val = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(config, "value", val);
    cJSON_AddNullToObject(config, "maybe");
    cJSON_AddStringToObject(config, "name", "test");
    cJSON *items = cJSON_AddArrayToObject(root, "items");
    cJSON *s1 = cJSON_CreateString("one");
    cJSON_AddItemToArray(items, s1);

    // step 2: Configure
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 3: Operate and Validate
    cJSON *maybe = cJSON_DetachItemFromObject(config, "maybe");
    cJSON_bool was_null = cJSON_IsNull(maybe);
    cJSON_bool is_obj = cJSON_IsObject(config);
    double status = (double)was_null + (double)is_obj;
    cJSON *computed = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(computed, 3.14159 * status);
    cJSON_AddItemToObject(root, "computed", computed);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(maybe);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}