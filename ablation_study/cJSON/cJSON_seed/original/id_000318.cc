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
//<ID> 318
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_AddArrayToObject(root, "numbers");
    cJSON *num_a = cJSON_CreateNumber(3.1415);
    cJSON *num_b = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num_a);
    cJSON_AddItemToArray(arr, num_b);

    // step 2: Configure
    cJSON *raw_item = cJSON_AddRawToObject(root, "raw_blob", "{\"x\":1}");
    cJSON_bool raw_flag = cJSON_IsRaw(raw_item);
    cJSON_bool invalid_flag = cJSON_IsInvalid(raw_item);
    cJSON *title_item = cJSON_AddStringToObject(root, "title", "example");
    const char *title_str = cJSON_GetStringValue(title_item);
    (void)title_str;
    (void)raw_flag;
    (void)invalid_flag;

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    int buf_len = 256;
    char *buffer = (char *)cJSON_malloc((size_t)buf_len);
    memset(buffer, 0, (size_t)buf_len);
    cJSON_PrintPreallocated(root, buffer, buf_len, 0);
    cJSON *duplicate = cJSON_Duplicate(root, 1);
    cJSON_bool equal = cJSON_Compare(root, duplicate, 1);
    (void)equal;
    cJSON_Delete(duplicate);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}