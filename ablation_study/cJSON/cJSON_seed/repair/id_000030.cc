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
//<ID> 30
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
    cJSON_AddItemToObject(root, "items", arr);
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON_AddItemToArray(arr, s1);

    // step 2: Configure
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, s2);
    char *res_set = cJSON_SetValuestring(s2, "beta_modified");
    (void)res_set;
    cJSON *s3 = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(arr, s3);

    // step 3: Operate and Validate
    int count = cJSON_GetArraySize(arr);
    cJSON_AddNumberToObject(root, "count", (double)count);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}