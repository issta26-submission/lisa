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
//<ID> 2454
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *list = cJSON_CreateArray();
    cJSON *s1 = cJSON_CreateString("alpha");
    cJSON *s2 = cJSON_CreateString("beta");
    cJSON_AddItemToArray(list, s1);
    cJSON_AddItemToArray(list, s2);
    cJSON_AddItemToObject(root, "list", list);
    cJSON_AddNumberToObject(root, "count", 2.0);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : '\0';
    cJSON *list_ref = cJSON_CreateArrayReference(list);
    cJSON_AddItemToObject(root, "list_ref", list_ref);

    // step 3: Operate
    cJSON *first_item = cJSON_GetArrayItem(list, 0);
    char *first_str = cJSON_GetStringValue(first_item);
    buffer[1] = first_str && first_str[0] ? first_str[0] : '\0';
    cJSON_bool has_list = cJSON_HasObjectItem(root, "list");
    buffer[2] = (char)(int)has_list;
    cJSON *ref_obj = cJSON_GetObjectItem(root, "list_ref");
    cJSON *ref_first = cJSON_GetArrayItem(ref_obj, 0);
    char *ref_first_str = cJSON_GetStringValue(ref_first);
    buffer[3] = ref_first_str && ref_first_str[0] ? ref_first_str[0] : '\0';

    // step 4: Validate and Cleanup
    cJSON *count_item = cJSON_GetObjectItem(root, "count");
    double count_val = cJSON_GetNumberValue(count_item);
    buffer[4] = (char)(int)count_val;
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}