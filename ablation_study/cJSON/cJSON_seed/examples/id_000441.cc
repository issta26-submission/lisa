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
//<ID> 441
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", numbers);

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *num2 = cJSON_CreateNumber(3.1415);
    cJSON *flag_false = cJSON_CreateFalse();
    cJSON *null_item = cJSON_CreateNull();
    cJSON_AddItemToArray(numbers, num1);
    cJSON_AddItemToArray(numbers, num2);
    cJSON_AddItemToArray(numbers, flag_false);
    cJSON_AddItemToArray(numbers, null_item);

    // step 3: Operate & Validate
    cJSON *retrieved = cJSON_GetObjectItem(root, "numbers");
    cJSON_bool null_check = cJSON_IsNull(cJSON_GetArrayItem(retrieved, 3));
    double v0 = cJSON_GetNumberValue(cJSON_GetArrayItem(retrieved, 0));
    double v1 = cJSON_GetNumberValue(cJSON_GetArrayItem(retrieved, 1));
    double sum = v0 + v1;
    char *snapshot = cJSON_PrintUnformatted(root);
    char info_buf[128];
    memset(info_buf, 0, sizeof(info_buf));
    sprintf(info_buf, "sum=%.4f|null=%d", sum, (int)null_check);
    cJSON_AddStringToObject(root, "info", info_buf);

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}