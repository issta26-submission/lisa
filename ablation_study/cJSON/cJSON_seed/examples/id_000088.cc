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
//<ID> 88
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "items", array);
    cJSON *label = cJSON_CreateString("example-label");
    cJSON_AddItemToObject(root, "label", label);

    // step 2: Configure
    cJSON *n1 = cJSON_CreateNumber(10.5);
    cJSON *n2 = cJSON_CreateNumber(4.25);
    cJSON_AddItemToArray(array, n1);
    cJSON_AddItemToArray(array, n2);

    // step 3: Operate & Validate
    cJSON *first = cJSON_GetArrayItem(array, 0);
    cJSON *second = cJSON_GetArrayItem(array, 1);
    double v0 = cJSON_GetNumberValue(first);
    double v1 = cJSON_GetNumberValue(second);
    cJSON *sum = cJSON_CreateNumber(v0 + v1);
    cJSON_AddItemToObject(root, "sum", sum);
    cJSON_bool has_label = cJSON_HasObjectItem(root, "label");
    cJSON *has_label_flag = cJSON_CreateNumber((double)has_label);
    cJSON *flags = cJSON_CreateArray();
    cJSON_AddItemToArray(flags, has_label_flag);
    cJSON_AddItemToObject(root, "flags", flags);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *buffer = (char *)cJSON_malloc(printed_len + 1);
    memset(buffer, 0, printed_len + 1);
    memcpy(buffer, printed, printed_len + 1);
    cJSON *root_dup = cJSON_Duplicate(root, 1);

    // step 4: Cleanup
    cJSON_Delete(root_dup);
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}