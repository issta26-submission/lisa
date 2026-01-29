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
//<ID> 292
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = cJSON_CreateObject();
    cJSON *arr = cJSON_CreateArray();
    cJSON *num = cJSON_CreateNumber(1.0);
    cJSON *raw = cJSON_CreateRaw("{\"raw_inner\":123}");
    char *printed = NULL;
    cJSON *parsed = NULL;
    cJSON *newnum = NULL;
    int arr_size = 0;

    // step 2: Setup / Configure
    cJSON_AddItemToObject(root, "arr", arr);
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(10));
    cJSON_AddItemToArray(arr, cJSON_CreateNumber(20));
    cJSON_AddItemToObject(root, "num", num);
    cJSON_AddItemToObject(root, "raw", raw);

    // step 3: Operate / Validate
    printed = cJSON_Print(root);
    parsed = cJSON_Parse(printed);
    newnum = cJSON_CreateNumber(99.5);
    cJSON_ReplaceItemInObject(parsed, "num", newnum);
    arr_size = cJSON_GetArraySize(cJSON_GetObjectItem(parsed, "arr"));
    (void)arr_size;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}