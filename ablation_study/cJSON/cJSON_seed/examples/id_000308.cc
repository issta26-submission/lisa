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
//<ID> 308
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
    cJSON *first = cJSON_CreateNumber(42.0);
    cJSON *second = cJSON_CreateNull();
    cJSON_AddItemToArray(arr, first);
    cJSON_AddItemToArray(arr, second);
    cJSON *raw = cJSON_CreateRaw("{\"raw_key\":123}");

    // step 2: Configure
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToObject(root, "raw_payload", raw);
    cJSON_bool has_data = cJSON_HasObjectItem(root, "data");
    cJSON_AddItemToObject(root, "has_data", cJSON_CreateBool(has_data));

    // step 3: Operate & Validate
    cJSON *second_item = cJSON_GetArrayItem(arr, 1);
    cJSON_bool second_is_null = cJSON_IsNull(second_item);
    cJSON_AddItemToObject(root, "second_is_null", cJSON_CreateBool(second_is_null));
    char *buf = (char*)cJSON_malloc((size_t)256);
    memset(buf, 0, 256);
    cJSON_PrintPreallocated(root, buf, 256, (cJSON_bool)1);
    cJSON_AddItemToObject(root, "printed", cJSON_CreateString(buf));

    // step 4: Cleanup
    cJSON_free(buf);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}