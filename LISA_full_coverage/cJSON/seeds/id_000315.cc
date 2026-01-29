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
//<ID> 315
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
    cJSON *n1 = cJSON_CreateNumber(3.14);
    cJSON *n2 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddStringToObject(root, "greeting", "hello");
    cJSON *str_item = cJSON_GetObjectItem(root, "greeting");
    char *str_val = (char *)cJSON_GetStringValue(str_item);

    // step 2: Configure
    cJSON *raw_item = cJSON_AddRawToObject(root, "raw_blob", "{\"x\":1}");
    cJSON_bool raw_is_raw = cJSON_IsRaw(raw_item);
    cJSON_AddBoolToObject(root, "raw_is_raw", raw_is_raw);
    cJSON *tmp = cJSON_CreateNumber(0.0);
    cJSON_bool tmp_invalid = cJSON_IsInvalid(tmp);
    cJSON_AddBoolToObject(root, "tmp_is_invalid", tmp_invalid);
    cJSON_Delete(tmp);

    // step 3: Operate and Validate
    char *printed = cJSON_PrintUnformatted(root);
    size_t printed_len = (printed ? strlen(printed) : 0);
    cJSON_AddNumberToObject(root, "printed_len", (double)printed_len);
    cJSON_AddStringToObject(root, "extracted_greeting", str_val);
    void *buffer = cJSON_malloc(128);
    memset(buffer, 0, 128);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}