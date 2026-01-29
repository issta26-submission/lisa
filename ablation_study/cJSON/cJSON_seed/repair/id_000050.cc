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
//<ID> 50
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
    cJSON *flag = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 2: Configure
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(items, s1);
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(items, s2);
    cJSON *s3 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(items, s3);

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(items);
    cJSON_AddNumberToObject(root, "count", (double)count);
    cJSON_bool is_false = cJSON_IsFalse(flag);
    cJSON_AddBoolToObject(root, "is_false", is_false);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}