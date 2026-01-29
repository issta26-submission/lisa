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
//<ID> 1205
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
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON *ref0 = cJSON_CreateStringReference("alpha");
    cJSON_AddItemToArray(arr, ref0);
    cJSON *ref1 = cJSON_CreateStringReference("beta");
    cJSON_InsertItemInArray(arr, 1, ref1);
    cJSON *normal = cJSON_CreateString("gamma");
    cJSON_AddItemToArray(arr, normal);

    // step 2: Configure
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON *num = cJSON_CreateNumber(42.0);
    cJSON_AddItemToObject(meta, "answer", num);
    cJSON_AddStringToObject(root, "status", "ok");

    // step 3: Operate
    char *pretty = cJSON_Print(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);
    int arr_size = cJSON_GetArraySize(arr);

    // step 4: Validate & Cleanup
    (void)arr_size;
    cJSON_free(pretty);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}