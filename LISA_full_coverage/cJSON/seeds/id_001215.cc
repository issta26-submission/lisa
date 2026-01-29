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
//<ID> 1215
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
    cJSON_AddNullToObject(root, "explicit_null");

    // step 2: Configure
    cJSON *s = cJSON_CreateString("initial");
    char *updated_value = cJSON_SetValuestring(s, "updated");
    cJSON *t = cJSON_CreateTrue();
    cJSON *n = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, s);
    cJSON_AddItemToArray(arr, t);
    cJSON_AddItemToArray(arr, n);

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    int arr_size = cJSON_GetArraySize(arr);
    int buf_len = 256;
    char *buf = (char *)cJSON_malloc((size_t)buf_len);
    memset(buf, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buf, buf_len, 1);

    // step 4: Validate & Cleanup
    (void)updated_value;
    (void)arr_size;
    cJSON_free(printed);
    cJSON_free(buf);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}