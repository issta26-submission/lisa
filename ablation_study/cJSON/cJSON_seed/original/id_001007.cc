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
//<ID> 1007
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char initial_json[] = "{\"numbers\":[1,2,3.5],\"info\":{\"name\":\"test\",\"active\":false},\"value\":10}";
    size_t initial_len = sizeof(initial_json) - 1;
    const char *parse_end = NULL;
    cJSON *root = cJSON_ParseWithLengthOpts(initial_json, initial_len, &parse_end, 1);

    // step 2: Configure
    cJSON *numbers = cJSON_GetObjectItem(root, "numbers");
    cJSON *first = cJSON_GetArrayItem(numbers, 0);
    cJSON *second = cJSON_GetArrayItem(numbers, 1);
    cJSON *third = cJSON_GetArrayItem(numbers, 2);
    cJSON_bool first_is_num = cJSON_IsNumber(first);
    cJSON_bool second_is_num = cJSON_IsNumber(second);
    double v0 = cJSON_GetNumberValue(first);
    double v1 = cJSON_GetNumberValue(second);
    double v2 = cJSON_GetNumberValue(third);
    double sum = v0 + v1 + v2;
    cJSON_AddNumberToObject(root, "sum", sum);
    cJSON_AddTrueToObject(root, "has_numbers");
    cJSON *temp = cJSON_CreateNumber(999);
    temp->type = 0;
    cJSON_bool temp_invalid = cJSON_IsInvalid(temp);
    (void)first_is_num;
    (void)second_is_num;
    (void)temp_invalid;

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    char *print_buf = (char *)cJSON_malloc(512);
    memset(print_buf, 0, 512);
    cJSON_PrintPreallocated(root, print_buf, 512, 0);

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObject(root, "value");
    cJSON_DeleteItemFromObject(root, "info");
    cJSON_free(out);
    cJSON_free(print_buf);
    cJSON_Delete(root);
    cJSON_Delete(temp);

    // API sequence test completed successfully
    return 66;
}