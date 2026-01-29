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
//<ID> 940
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
    cJSON_AddNumberToObject(root, "count", 2);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON_InsertItemInArray(arr, 0, n1);
    cJSON *n2 = cJSON_CreateNumber(3.14);
    cJSON_InsertItemInArray(arr, 1, n2);

    // step 3: Operate
    cJSON *got = cJSON_GetArrayItem(arr, 1);
    double picked = cJSON_GetNumberValue(got);
    cJSON_AddNumberToObject(root, "picked", picked);

    // step 4: Validate & Cleanup
    int bufsize = 256;
    char *buffer = (char *)cJSON_malloc((size_t)bufsize);
    memset(buffer, 0, (size_t)bufsize);
    cJSON_PrintPreallocated(root, buffer, bufsize, 1);
    cJSON_free(buffer);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}