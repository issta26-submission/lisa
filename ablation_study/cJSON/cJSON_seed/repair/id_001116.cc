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
//<ID> 1116
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *falseItem = cJSON_CreateFalse();
    cJSON_AddItemToArray(arr, falseItem);
    cJSON *strItem = cJSON_CreateString("example");
    cJSON_AddItemToArray(arr, strItem);
    cJSON_AddItemToObject(root, "payload", arr);

    // step 2: Configure
    cJSON_bool is_bool = cJSON_IsBool(falseItem);
    cJSON_bool is_str = cJSON_IsString(strItem);
    double computed = 42.0 + (double)(is_bool) * 1.0 + (double)(is_str) * 2.0;
    cJSON *computed_item = cJSON_CreateNumber(computed);
    cJSON_AddItemToObject(root, "computed", computed_item);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(64);
    memset(scratch, 0, 64);
    ((char *)scratch)[0] = out ? out[0] : '\0';
    ((char *)scratch)[1] = out ? out[1] : '\0';
    ((char *)scratch)[2] = (char)((int)computed & 0xFF);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}