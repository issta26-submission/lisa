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
//<ID> 1200
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
    cJSON_AddItemToObject(root, "letters", arr);

    // step 2: Configure
    cJSON *s1 = cJSON_CreateStringReference("alpha");
    cJSON *s2 = cJSON_CreateStringReference("beta");
    cJSON *s3 = cJSON_CreateStringReference("gamma");
    cJSON_bool ok1 = cJSON_InsertItemInArray(arr, 0, s1);
    cJSON_bool ok2 = cJSON_InsertItemInArray(arr, 1, s2);
    cJSON_bool ok3 = cJSON_InsertItemInArray(arr, 2, s3);

    // step 3: Operate
    char *pretty = cJSON_Print(root);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_bool pre_ok = cJSON_PrintPreallocated(root, buf, buf_len, 1);

    // step 4: Validate & Cleanup
    int arr_size = cJSON_GetArraySize(arr);
    (void)ok1;
    (void)ok2;
    (void)ok3;
    (void)pre_ok;
    (void)arr_size;
    cJSON_free(pretty);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}