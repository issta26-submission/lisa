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
//<ID> 1102
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
    cJSON_AddStringToObject(meta, "id", "meta1");
    cJSON *childArr = cJSON_CreateArray();
    cJSON_AddItemToArray(childArr, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(childArr, cJSON_CreateNumber(2.0));
    cJSON *arrRef = cJSON_CreateArrayReference(childArr);
    cJSON_AddItemToObject(root, "shared_array_ref", arrRef);
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "enabled", flag);

    // step 2: Configure
    cJSON_AddStringToObject(meta, "status", "ok");
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);

    // step 3: Operate and Validate
    cJSON_bool is_bool = cJSON_IsBool(flag);
    cJSON_bool is_false = cJSON_IsFalse(flag);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = (char)('0' + (is_bool ? 1 : 0));
    ((char *)scratch)[2] = (char)('0' + (is_false ? 1 : 0));

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    cJSON_Delete(childArr);
    // API sequence test completed successfully
    return 66;
}