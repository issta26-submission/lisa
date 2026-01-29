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
//<ID> 1480
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
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(2.0);
    cJSON_AddItemToArray(values, n1);
    cJSON_AddItemToArray(values, n2);
    cJSON_AddItemToObject(root, "values", values);

    // step 2: Configure
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed) + 1;
    void *buffer = cJSON_malloc(printed_len);
    memset(buffer, 0, printed_len);
    memcpy(buffer, printed, printed_len);

    // step 3: Operate & Validate
    cJSON_bool is_array = cJSON_IsArray(values);
    cJSON *first_item = cJSON_GetArrayItem(values, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    cJSON *doubled = cJSON_CreateNumber(first_value * 2.0);
    cJSON_AddItemToObject(root, "doubled_first", doubled);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    (void)is_array;
    (void)first_value;
    return 66;
}