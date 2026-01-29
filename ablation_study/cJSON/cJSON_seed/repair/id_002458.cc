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
//<ID> 2458
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
    cJSON *s0 = cJSON_CreateString("alpha");
    cJSON *s1 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, s0);
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToObject(root, "list", arr);
    cJSON *version_item = cJSON_AddNumberToObject(root, "version", 1.0);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : '\0';

    // step 3: Operate
    cJSON *arr_ref = cJSON_CreateArrayReference(arr);
    cJSON *first_item = cJSON_GetArrayItem(arr_ref, 0);
    char *first_str = cJSON_GetStringValue(first_item);
    cJSON_bool has_list = cJSON_HasObjectItem(root, "list");
    buffer[1] = first_str && first_str[0] ? first_str[0] : '\0';
    buffer[2] = (char)(int)has_list;
    cJSON_AddNumberToObject(root, "added", 42.0);

    // step 4: Validate and Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(arr_ref);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}