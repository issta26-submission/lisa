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
//<ID> 584
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
    cJSON *n0 = cJSON_CreateNumber(1.5);
    cJSON *n1 = cJSON_CreateNumber(3.5);
    cJSON_AddItemToArray(numbers, n0);
    cJSON_InsertItemInArray(numbers, 1, n1);
    cJSON_AddNumberToObject(root, "initial_count", 2.0);

    // step 2: Configure
    cJSON_AddNumberToObject(root, "scale", 2.0);
    cJSON *scaled0 = cJSON_CreateNumber(1.5 * 2.0);
    cJSON_AddItemToArray(numbers, scaled0);

    // step 3: Operate and Validate
    cJSON *got_numbers = cJSON_GetObjectItem(root, "numbers");
    cJSON *got_numbers_cs = cJSON_GetObjectItemCaseSensitive(root, "numbers");
    cJSON_bool has_nums = cJSON_HasObjectItem(root, "numbers");
    cJSON *item0 = cJSON_GetArrayItem(got_numbers, 0);
    cJSON *item1 = cJSON_GetArrayItem(got_numbers, 1);
    cJSON *item2 = cJSON_GetArrayItem(got_numbers, 2);
    double v0 = cJSON_GetNumberValue(item0);
    double v1 = cJSON_GetNumberValue(item1);
    double v2 = cJSON_GetNumberValue(item2);
    double sum = v0 + v1 + v2 + (has_nums ? 0.0 : 0.0) + (got_numbers_cs ? 0.0 : 0.0);
    cJSON_AddNumberToObject(root, "sum", sum);
    char *out = cJSON_PrintUnformatted(root);
    cJSON_free(out);

    // step 4: Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}