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
//<ID> 2321
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
    cJSON_AddItemToObject(root, "arr", array);
    cJSON *strref = cJSON_CreateStringReference("ref-string");
    cJSON_AddItemToArray(array, strref);
    cJSON *num = cJSON_CreateNumber(42.5);
    cJSON_AddItemToArray(array, num);

    // step 2: Configure
    cJSON *dup_array = cJSON_Duplicate(array, 1);
    cJSON_AddItemToObject(root, "arr_copy", dup_array);
    cJSON_AddNumberToObject(root, "version", 1.0);

    // step 3: Operate
    char *json = cJSON_PrintUnformatted(root);
    cJSON *first_item = cJSON_GetArrayItem(array, 0);
    const char *first_str = cJSON_GetStringValue(first_item);
    cJSON *second_item = cJSON_GetArrayItem(array, 1);
    double second_num = cJSON_GetNumberValue(second_item);

    // step 4: Validate and Cleanup
    const int buflen = 128;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);
    buffer[0] = (json && json[0]) ? json[0] : '\0';
    buffer[1] = (first_str && first_str[0]) ? first_str[0] : '\0';
    buffer[2] = (char)(((int)second_num) + '0');
    cJSON_free(json);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}