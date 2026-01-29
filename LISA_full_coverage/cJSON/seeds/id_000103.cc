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
//<ID> 103
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
    char raw_json[] = " { \"alpha\" : 10 , \"beta\" : false } ";
    cJSON_Minify(raw_json);
    cJSON *parsed = cJSON_Parse(raw_json);
    cJSON_AddItemToObject(root, "parsed", parsed);

    // step 2: Configure
    cJSON *flag = cJSON_AddTrueToObject(root, "enabled");
    cJSON *str0 = cJSON_CreateString("first");
    cJSON_InsertItemInArray(arr, 0, str0);
    cJSON *num = cJSON_CreateNumber(3.14159);
    cJSON_AddItemToArray(arr, num);
    cJSON *raw = cJSON_CreateRaw("{\"raw\":1}");
    cJSON_AddItemToArray(arr, raw);

    // step 3: Operate and Validate
    char *printed = cJSON_Print(root);
    cJSON_Minify(printed);
    int count = cJSON_GetArraySize(arr);
    double alpha_val = cJSON_GetNumberValue(cJSON_GetObjectItem(parsed, "alpha"));
    (void)flag;
    (void)count;
    (void)alpha_val;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}