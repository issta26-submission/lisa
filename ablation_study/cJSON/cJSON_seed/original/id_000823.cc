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
//<ID> 823
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
    cJSON *num0 = cJSON_CreateNumber(42.0);
    cJSON *str1 = cJSON_CreateString("hello");
    cJSON *ins = cJSON_CreateString("inserted");
    cJSON_AddItemToObject(root, "data", arr);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToArray(arr, str1);
    cJSON_InsertItemInArray(arr, 1, ins);
    cJSON_AddStringToObject(root, "version", cJSON_Version());
    cJSON_AddNumberToObject(root, "temp", 3.14);

    // step 2: Configure
    cJSON_SetNumberHelper(num0, 100.0);

    // step 3: Operate
    char *pretty = cJSON_Print(root);
    char *compact = cJSON_PrintUnformatted(root);
    cJSON_free(pretty);
    cJSON_free(compact);
    cJSON *got_version = cJSON_GetObjectItem(root, "version");
    const char *verstr = cJSON_GetStringValue(got_version);
    cJSON *got_arr = cJSON_GetObjectItem(root, "data");
    double first_val = cJSON_GetNumberValue(cJSON_GetArrayItem(got_arr, 0));
    const char *third_str = cJSON_GetStringValue(cJSON_GetArrayItem(got_arr, 2));
    (void)verstr;
    (void)first_val;
    (void)third_str;

    // step 4: Validate & Cleanup
    cJSON_DeleteItemFromObjectCaseSensitive(root, "temp");
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}