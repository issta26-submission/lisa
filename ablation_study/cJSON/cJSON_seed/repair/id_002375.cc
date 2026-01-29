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
//<ID> 2375
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
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);

    // step 2: Configure
    cJSON *arr_ref = cJSON_CreateArrayReference(arr);
    cJSON_AddItemToObject(root, "numbers", arr_ref);
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "meta", meta);
    cJSON_AddItemToObject(meta, "name", cJSON_CreateString("example"));

    // step 3: Operate
    char *printed = cJSON_PrintUnformatted(root);
    cJSON *got = cJSON_GetObjectItem(root, "numbers");
    cJSON_bool has_numbers = cJSON_HasObjectItem(root, "numbers");
    cJSON *got_meta = cJSON_GetObjectItem(root, "meta");
    cJSON_bool meta_is_obj = cJSON_IsObject(got_meta);
    cJSON *first = cJSON_GetArrayItem(got, 0);
    double first_val = cJSON_GetNumberValue(first);

    // step 4: Validate and Cleanup
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed[0];
    buffer[1] = (char)(int)first_val;
    buffer[2] = (char)(int)has_numbers;
    buffer[3] = (char)(int)meta_is_obj;
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}