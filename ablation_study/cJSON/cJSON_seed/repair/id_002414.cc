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
//<ID> 2414
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_CreateArray();
    cJSON *num0 = cJSON_CreateNumber(42.0);
    cJSON *str1 = cJSON_CreateString("example");
    cJSON_AddItemToArray(values, num0);
    cJSON_AddItemToArray(values, str1);
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    cJSON *dup_root = cJSON_Duplicate(root, 1);
    cJSON *dup_values = cJSON_GetObjectItem(dup_root, "values");
    cJSON *second_item = cJSON_GetArrayItem(dup_values, 1);
    char *printed_dup = cJSON_PrintUnformatted(dup_root);
    const int buflen = 64;
    char *buffer = (char *)cJSON_malloc((size_t)buflen);
    memset(buffer, 0, (size_t)buflen);

    // step 3: Operate
    double first_number = cJSON_GetNumberValue(cJSON_GetArrayItem(values, 0));
    char *second_string = cJSON_GetStringValue(second_item);
    buffer[0] = printed_dup && printed_dup[0] ? printed_dup[0] : '\0';
    buffer[1] = (char)(int)first_number;
    buffer[2] = second_string && second_string[0] ? second_string[0] : '\0';

    // step 4: Validate and Cleanup
    cJSON_free(printed_dup);
    cJSON_free(buffer);
    cJSON_Delete(root);
    cJSON_Delete(dup_root);
    // API sequence test completed successfully
    return 66;
}