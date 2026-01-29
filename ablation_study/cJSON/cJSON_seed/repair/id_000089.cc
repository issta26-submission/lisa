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
//<ID> 89
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateArray();
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(root, n1);
    cJSON *n2 = cJSON_CreateNumber(1.86);
    cJSON_AddItemToArray(root, n2);

    // step 2: Configure
    cJSON *dup = cJSON_Duplicate(root, 1);
    cJSON *n3 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(dup, n3);

    // step 3: Operate and Validate
    int sz_root = cJSON_GetArraySize(root);
    cJSON *first = cJSON_GetArrayItem(root, 0);
    double v0 = cJSON_GetNumberValue(first);
    cJSON *first_dup = cJSON_GetArrayItem(dup, 0);
    double v0dup = cJSON_GetNumberValue(first_dup);
    double sum = v0 + v0dup;
    cJSON *sumItem = cJSON_CreateNumber(sum);
    cJSON_AddItemToArray(root, sumItem);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(dup);
    cJSON_Delete(root);
    return 66;
    // API sequence test completed successfully
}