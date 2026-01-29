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
//<ID> 1263
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *child = cJSON_AddObjectToObject(root, "child");
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToObject(child, "arr", arr);
    cJSON *n1 = cJSON_CreateNumber(1);
    cJSON *n2 = cJSON_CreateNumber(2);
    cJSON *n3 = cJSON_CreateNumber(3);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToArray(arr, n3);

    // step 2: Configure
    int arr_size = cJSON_GetArraySize(arr);
    char *size_buf = (char *)cJSON_malloc(4);
    memset(size_buf, 0, 4);
    size_buf[0] = '0' + (char)arr_size;
    cJSON *size_str = cJSON_CreateString(size_buf);
    cJSON_free(size_buf);
    cJSON_AddItemToObject(root, "size", size_str);
    cJSON *label = cJSON_CreateString("sample");
    cJSON_AddItemToObject(root, "label", label);

    // step 3: Operate and Validate
    char *out = cJSON_PrintUnformatted(root);
    void *scratch = cJSON_malloc(8);
    memset(scratch, 0, 8);
    ((char *)scratch)[0] = out[0];
    ((char *)scratch)[1] = out[1];

    // step 4: Cleanup
    cJSON_free(out);
    cJSON_free(scratch);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}