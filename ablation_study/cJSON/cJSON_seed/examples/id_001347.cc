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
//<ID> 1347
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
    cJSON *raw_init = cJSON_CreateRaw("{\"embedded\":123}");
    cJSON *s_item = cJSON_CreateString("alpha");
    cJSON *n_item = cJSON_CreateNumber(7.0);

    // step 2: Configure
    cJSON_AddRawToObject(root, "raw_entry", "{\"nested\":true}");
    cJSON_AddFalseToObject(root, "flag");
    cJSON_AddItemToArray(arr, s_item);
    cJSON_AddItemToArray(arr, n_item);
    cJSON_AddItemToArray(arr, cJSON_CreateString("beta"));
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToObject(root, "init_raw", raw_init);

    // step 3: Operate & Validate
    int count = cJSON_GetArraySize(arr);
    cJSON *count_num = cJSON_CreateNumber((double)count);
    cJSON_AddItemToObject(root, "count", count_num);
    int buf_len = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 1);
    cJSON_AddRawToObject(root, "printed", buffer);

    // step 4: Cleanup
    cJSON_Delete(root);
    cJSON_free(buffer);
    // API sequence test completed successfully
    return 66;
}