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
//<ID> 2451
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *version_item = cJSON_AddNumberToObject(root, "version", 1.0);
    cJSON *arr = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(arr, s1);
    cJSON_AddItemToArray(arr, s2);
    cJSON *ref = cJSON_CreateArrayReference(arr);
    cJSON_AddItemToObject(root, "list", ref);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : '\0';

    // step 3: Operate
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    char *first_str = (char *)cJSON_GetStringValue(first_item);
    buffer[1] = first_str && first_str[0] ? first_str[0] : '\0';
    cJSON_bool has_version = cJSON_HasObjectItem(root, "version");
    buffer[2] = (char)(int)has_version;
    cJSON *second_item_from_ref = cJSON_GetArrayItem(ref, 1);
    char *second_str = (char *)cJSON_GetStringValue(second_item_from_ref);
    buffer[3] = second_str && second_str[0] ? second_str[0] : '\0';
    cJSON *count_item = cJSON_AddNumberToObject(root, "count", 2.0);

    // step 4: Validate and Cleanup
    char *printed2 = cJSON_PrintUnformatted(root);
    buffer[4] = printed2 && printed2[0] ? printed2[0] : '\0';
    cJSON_free(printed);
    cJSON_free(printed2);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}