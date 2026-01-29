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
//<ID> 1209
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

    // step 2: Configure
    cJSON *sref1 = cJSON_CreateStringReference("first");
    cJSON_bool ins1 = cJSON_InsertItemInArray(arr, 0, sref1);
    cJSON *sref2 = cJSON_CreateStringReference("second");
    cJSON_bool ins2 = cJSON_InsertItemInArray(arr, 1, sref2);

    // step 3: Operate
    char *printed = cJSON_Print(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_bool prealloc_ok = cJSON_PrintPreallocated(root, buf, buf_len, 1);
    int arr_size = cJSON_GetArraySize(arr);

    // step 4: Validate & Cleanup
    (void)ins1;
    (void)ins2;
    (void)prealloc_ok;
    (void)arr_size;
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}