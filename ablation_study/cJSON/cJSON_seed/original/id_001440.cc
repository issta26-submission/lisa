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
//<ID> 1440
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    cJSON *root = cJSON_CreateObject();
    cJSON *numbers = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", numbers);
    cJSON *n0 = cJSON_CreateNumber(1.0);
    cJSON *n1 = cJSON_CreateNumber(3.0);
    cJSON_AddItemToArray(numbers, n0);
    cJSON_AddItemToArray(numbers, n1);
    cJSON *n_insert = cJSON_CreateNumber(2.0);
    cJSON_InsertItemInArray(numbers, 1, n_insert);

    cJSON *meta = cJSON_AddObjectToObject(root, "meta");
    cJSON_AddStringToObject(meta, "status", "ok");
    cJSON *numbers_copy = cJSON_Duplicate(numbers, 1);
    cJSON_AddItemToObject(meta, "numbers_ref", numbers_copy);

    cJSON_bool numbers_is_array = cJSON_IsArray(numbers);
    cJSON *first_elem = cJSON_GetArrayItem(numbers, 0);
    cJSON_bool first_is_invalid = cJSON_IsInvalid(first_elem);
    double first_value = cJSON_GetNumberValue(first_elem);
    double second_value = cJSON_GetNumberValue(cJSON_GetArrayItem(numbers, 1));
    int numbers_count = cJSON_GetArraySize(numbers);
    char *printed = cJSON_PrintUnformatted(root);
    int buf_len = 512;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 0);
    const char *status = cJSON_GetStringValue(cJSON_GetObjectItem(meta, "status"));
    (void)numbers_is_array;
    (void)first_is_invalid;
    (void)first_value;
    (void)second_value;
    (void)numbers_count;
    (void)status;

    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    return 66;
}