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
//<ID> 782
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    const char *json = "{\"value\": 42}";
    cJSON *parsed = NULL;
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *num0 = NULL;
    cJSON *num1 = NULL;
    cJSON *extracted = NULL;
    char *printed = NULL;
    char *copy = NULL;
    size_t printed_len = 0;
    double val = 0.0;

    // step 2: Setup / Configure
    parsed = cJSON_Parse(json);
    extracted = cJSON_GetObjectItem(parsed, "value");
    val = cJSON_GetNumberValue(extracted);
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    num0 = cJSON_CreateNumber(val);
    cJSON_AddItemToArray(arr, num0);
    cJSON_AddItemToObject(root, "arr", arr);

    // step 3: Operate / Validate
    num1 = cJSON_CreateNumber(val + 58.0);
    cJSON_ReplaceItemInArray(arr, 0, num1);
    printed = cJSON_PrintUnformatted(root);
    printed_len = strlen(printed);
    copy = (char *)cJSON_malloc(printed_len + 1);
    memset(copy, 0, printed_len + 1);
    memcpy(copy, printed, printed_len + 1);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_free(copy);
    cJSON_Delete(parsed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}