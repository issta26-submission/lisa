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
//<ID> 1496
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *arr = NULL;
    cJSON *enabled = NULL;
    cJSON *num1 = NULL;
    cJSON *str1 = NULL;
    cJSON *parsed = NULL;
    cJSON *first = NULL;
    cJSON *second = NULL;
    char *printed = NULL;
    char json[] = " { \"embedded\" : true , \"empty\" : null, \"value\": 7 } ";
    char scratch[128];
    memset(scratch, 0, sizeof(scratch));

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    arr = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "list", arr);
    enabled = cJSON_AddBoolToObject(root, "enabled", 1);
    num1 = cJSON_CreateNumber(42.0);
    cJSON_AddItemToArray(arr, num1);
    str1 = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, str1);
    parsed = cJSON_Parse(json);
    cJSON_AddItemToObject(root, "parsed", parsed);

    // step 3: Operate / Validate
    first = cJSON_GetArrayItem(arr, 0);
    second = cJSON_GetArrayItem(arr, 1);
    (void)cJSON_GetObjectItem(root, "enabled");
    printed = cJSON_PrintUnformatted(root);
    (void)first;
    (void)second;
    (void)enabled;
    (void)scratch;

    // step 4: Cleanup
    if (printed) { cJSON_free(printed); printed = NULL; }
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}