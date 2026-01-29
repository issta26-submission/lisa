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
//<ID> 63
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *meta = cJSON_CreateObject();
    cJSON_AddItemToObjectCS(root, "meta", meta);

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(3.5);
    cJSON *num2 = cJSON_CreateNumber(2.5);
    cJSON_AddItemToObject(root, "value1", num1);
    cJSON_AddItemToObjectCS(root, "value2", num2);

    // step 3: Operate & Validate
    cJSON_bool has_value1 = cJSON_HasObjectItem(root, "value1");
    cJSON_bool has_value3 = cJSON_HasObjectItem(root, "value3");
    double sum_val = cJSON_GetNumberValue(num1) + cJSON_GetNumberValue(num2);
    cJSON *sum = cJSON_CreateNumber(sum_val);
    cJSON_AddItemToObjectCS(meta, "sum", sum);
    cJSON *flag1 = cJSON_CreateNumber((double)has_value1);
    cJSON *flag3 = cJSON_CreateNumber((double)has_value3);
    cJSON_AddItemToObject(root, "has_value1", flag1);
    cJSON_AddItemToObject(root, "has_value3", flag3);

    // step 4: Cleanup
    char *printed = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);
    cJSON_free(printed);

    // API sequence test completed successfully
    return 66;
}