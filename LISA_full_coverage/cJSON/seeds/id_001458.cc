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
//<ID> 1458
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char json_text[] = "{\"a\":1,\"arr\":[2,3]}";
    cJSON *root = cJSON_ParseWithLength(json_text, sizeof(json_text) - 1);
    cJSON *a_item = cJSON_GetObjectItem(root, "a");
    double a_val = cJSON_GetNumberValue(a_item);

    // step 2: Configure
    cJSON *num_created = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(num_created, a_val + 10.5);
    cJSON *arr = cJSON_GetObjectItem(root, "arr");
    cJSON_AddItemToArray(arr, num_created);
    cJSON *arr_ref = cJSON_CreateArrayReference(arr);

    // step 3: Operate & Validate
    cJSON_AddItemToObject(root, "arr_ref", arr_ref);
    cJSON_bool has_a = cJSON_HasObjectItem(root, "a");
    int buf_len = 512 + (int)has_a;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_bool printed_ok = cJSON_PrintPreallocated(root, buffer, buf_len, has_a);
    (void)printed_ok;

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}