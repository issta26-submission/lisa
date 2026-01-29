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
//<ID> 327
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *nested = cJSON_CreateObject();
    cJSON *num = cJSON_CreateNumber(10.0);
    cJSON *nul = cJSON_CreateNull();
    cJSON *str = cJSON_CreateString("hello");
    cJSON *arr = cJSON_CreateArray();
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(1.0));
    cJSON_AddItemToArray(arr, cJSON_CreateString("two"));
    cJSON_AddItemToObject(root, "num", num);
    cJSON_AddItemToObject(root, "nullable", nul);
    cJSON_AddItemToObject(root, "greeting", str);
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddItemToObject(root, "nested", nested);

    // step 2: Configure
    cJSON_AddNumberToObject(nested, "initial", 123.0);
    cJSON_SetNumberHelper(num, 100.5);

    // step 3: Operate & Validate
    double current = cJSON_GetNumberValue(num);
    char *printed = cJSON_Print(root);
    cJSON_AddStringToObject(root, "printed", printed);
    cJSON_DeleteItemFromObjectCaseSensitive(root, "nullable");

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    return 66; // API sequence test completed successfully
}