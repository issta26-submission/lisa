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
//<ID> 950
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();

    // step 2: Configure
    cJSON *sub = cJSON_AddObjectToObject(root, "sub");
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(sub, "arr", arr);
    cJSON *sref0 = cJSON_CreateStringReference("alpha");
    cJSON_InsertItemInArray(arr, 0, sref0);
    cJSON *sref1 = cJSON_CreateStringReference("beta");
    cJSON_InsertItemInArray(arr, 1, sref1);

    // step 3: Operate
    cJSON *arr_ref = cJSON_CreateArrayReference(arr);
    cJSON_AddItemToObject(root, "arr_ref", arr_ref);
    char *out = cJSON_PrintUnformatted(root);

    // step 4: Validate & Cleanup
    size_t buf_size = 128;
    void *buf = cJSON_malloc(buf_size);
    memset(buf, 0, buf_size);
    cJSON_PrintPreallocated(root, (char *)buf, (int)buf_size, 1);
    cJSON_free(out);
    cJSON_free(buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}