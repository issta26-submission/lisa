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
//<ID> 309
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
    cJSON *raw_item = cJSON_CreateRaw("{\"raw_key\":true}");
    cJSON_AddItemToArray(arr, raw_item);
    cJSON_AddItemToObject(root, "data", arr);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddNumberToObject(meta, "count", 1.0);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 2: Configure
    cJSON_bool has_data = cJSON_HasObjectItem(root, "data");
    cJSON *has_data_item = cJSON_CreateBool(has_data);
    cJSON_AddItemToObject(root, "has_data", has_data_item);
    cJSON_AddNumberToObject(root, "version_major", 1.0);

    // step 3: Operate & Validate
    cJSON *first_element = cJSON_GetArrayItem(arr, 0);
    cJSON_bool first_is_null = cJSON_IsNull(first_element);
    cJSON *first_is_null_item = cJSON_CreateBool(first_is_null);
    cJSON_AddItemToObject(root, "first_is_null", first_is_null_item);
    int buf_len = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, (cJSON_bool)1);
    char *printed_unformatted = cJSON_PrintUnformatted(root);
    cJSON_AddItemToObject(root, "printed_unformatted", cJSON_CreateString(printed_unformatted));

    // step 4: Cleanup
    cJSON_free(printed_unformatted);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}