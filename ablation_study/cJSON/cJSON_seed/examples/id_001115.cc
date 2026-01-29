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
//<ID> 1115
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
    cJSON *num0 = cJSON_CreateNumber(3.14);
    cJSON *num1 = cJSON_CreateNumber(2.0);
    cJSON *sref = cJSON_CreateStringReference("example_label");

    // step 2: Configure
    cJSON_AddItemToObject(root, "items", arr);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToObject(root, "name", sref);
    cJSON *active_item = cJSON_AddTrueToObject(root, "active");

    // step 3: Operate & Validate
    cJSON *second = cJSON_GetArrayItem(arr, 1);
    double before = cJSON_GetNumberValue(second);
    double after = cJSON_SetNumberHelper(second, 5.5);
    cJSON_bool is_num = cJSON_IsNumber(second);
    cJSON_AddNumberToObject(root, "original_second", before);
    cJSON_AddNumberToObject(root, "updated_second", after);
    cJSON_AddNumberToObject(root, "second_is_number", (double)is_num);

    // step 4: Cleanup
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);
    cJSON_Delete(root);
    (void)active_item; // maintain use of the returned pointer without logic branches

    return 66; // API sequence test completed successfully
}