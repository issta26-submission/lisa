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
//<ID> 840
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
    cJSON_AddItemToObject(root, "items", arr);
    char json_buf[] = " { \"x\": 10, \"y\": [1, 2] } ";
    cJSON_Minify(json_buf);
    cJSON *parsed = cJSON_Parse(json_buf);
    cJSON_AddItemToArray(arr, parsed);

    // step 2: Configure
    cJSON *num1 = cJSON_CreateNumber(0.0);
    cJSON_SetNumberHelper(num1, 123.456);
    cJSON_AddItemToObject(root, "number", num1);
    cJSON *num2 = cJSON_CreateNumber(999.0);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "number", num2);
    cJSON *obj = cJSON_CreateObject();
    cJSON_AddStringToObject(obj, "label", "example");
    cJSON_AddItemToArray(arr, obj);

    // step 3: Operate
    char *out = cJSON_PrintUnformatted(root);
    double number_value = cJSON_GetNumberValue(cJSON_GetObjectItemCaseSensitive(root, "number"));
    int array_size = cJSON_GetArraySize(arr);
    (void)number_value;
    (void)array_size;
    cJSON_free(out);

    // step 4: Validate & Cleanup
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}