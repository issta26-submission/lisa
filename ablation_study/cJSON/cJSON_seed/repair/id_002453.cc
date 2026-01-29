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
//<ID> 2453
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
    cJSON *s0 = cJSON_CreateString("alpha");
    cJSON *n1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(list, s0);
    cJSON_AddItemToArray(list, n1);
    cJSON_AddItemToObject(root, "list", list);
    cJSON *total_item = cJSON_AddNumberToObject(root, "total", 2.0);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = printed && printed[0] ? printed[0] : '\0';
    cJSON *list_ref = cJSON_CreateArrayReference(list);

    // step 3: Operate
    cJSON *first_in_ref = cJSON_GetArrayItem(list_ref, 0);
    cJSON *second_in_orig = cJSON_GetArrayItem(list, 1);
    char *first_str = cJSON_GetStringValue(first_in_ref);
    double second_val = cJSON_GetNumberValue(second_in_orig);
    cJSON_bool has_total = cJSON_HasObjectItem(root, "total");
    cJSON *added = cJSON_AddNumberToObject(root, "incremented", second_val + 10.0);

    // step 4: Validate and Cleanup
    buffer[1] = first_str && first_str[0] ? first_str[0] : '\0';
    buffer[2] = (char)(int)second_val;
    buffer[3] = (char)(int)has_total;
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(list_ref);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}