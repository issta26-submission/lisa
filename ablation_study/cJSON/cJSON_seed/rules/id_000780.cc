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
//<ID> 780
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *input_json = "{\"values\": [1.5, 2.5, 3.5]}";
    size_t input_len = strlen(input_json);
    char *buffer = (char *)cJSON_malloc(input_len + 1);
    memset(buffer, 0, input_len + 1);
    memcpy(buffer, input_json, input_len);
    buffer[input_len] = '\0';
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_values = cJSON_GetObjectItem(parsed, "values");
    cJSON *p0 = cJSON_GetArrayItem(parsed_values, 0);
    cJSON *p1 = cJSON_GetArrayItem(parsed_values, 1);
    cJSON *p2 = cJSON_GetArrayItem(parsed_values, 2);
    double pv0 = cJSON_GetNumberValue(p0);
    double pv1 = cJSON_GetNumberValue(p1);
    double pv2 = cJSON_GetNumberValue(p2);

    // step 2: Setup / Configure
    cJSON *root = cJSON_CreateObject();
    cJSON *array = cJSON_CreateArray();
    cJSON *n0 = cJSON_CreateNumber(pv0);
    cJSON *n1 = cJSON_CreateNumber(pv1);
    cJSON *n2 = cJSON_CreateNumber(pv2);
    cJSON_AddItemToArray(array, n0);
    cJSON_AddItemToArray(array, n1);
    cJSON_AddItemToArray(array, n2);
    cJSON_AddItemToObject(root, "arr", array);

    // step 3: Operate / Validate
    cJSON *replacement = cJSON_CreateNumber(4.5);
    cJSON_ReplaceItemInArray(array, 1, replacement);
    cJSON *item0 = cJSON_GetArrayItem(array, 0);
    cJSON *item1 = cJSON_GetArrayItem(array, 1);
    cJSON *item2 = cJSON_GetArrayItem(array, 2);
    double v0 = cJSON_GetNumberValue(item0);
    double v1 = cJSON_GetNumberValue(item1);
    double v2 = cJSON_GetNumberValue(item2);
    double sum = v0 + v1 + v2;
    cJSON *sum_item = cJSON_CreateNumber(sum);
    cJSON_AddItemToObject(root, "sum", sum_item);
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = strlen(printed);
    char *copied = (char *)cJSON_malloc(printed_len + 1);
    memset(copied, 0, printed_len + 1);
    memcpy(copied, printed, printed_len + 1);

    // step 4: Cleanup
    cJSON_free(buffer);
    cJSON_free(printed);
    cJSON_free(copied);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}