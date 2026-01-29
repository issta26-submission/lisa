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
//<ID> 316
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *values = cJSON_AddArrayToObject(root, "values");
    cJSON *num1 = cJSON_CreateNumber(3.14);
    cJSON *num2 = cJSON_CreateNumber(2.718);
    cJSON_AddItemToArray(values, num1);
    cJSON_AddItemToArray(values, num2);
    cJSON *label = cJSON_AddStringToObject(root, "label", "example");

    // step 2: Configure
    cJSON *raw = cJSON_AddRawToObject(root, "raw_blob", "{\"x\":1}");
    cJSON *extra = cJSON_AddArrayToObject(root, "extra");
    cJSON_AddItemToArray(extra, cJSON_CreateNumber(42.0));

    // step 3: Operate and Validate
    char *label_text = cJSON_GetStringValue(label);
    (void)label_text;
    cJSON_bool is_raw = cJSON_IsRaw(raw);
    (void)is_raw;
    cJSON_bool invalid_check = cJSON_IsInvalid(num1);
    (void)invalid_check;
    char *printed = cJSON_PrintUnformatted(root);
    void *buffer = cJSON_malloc(128);
    memset(buffer, 0, 128);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(buffer);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}