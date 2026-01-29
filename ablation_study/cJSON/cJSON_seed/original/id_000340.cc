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
//<ID> 340
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
    cJSON *n1 = cJSON_CreateNumber(10.0);
    cJSON *n2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(arr, n1);
    cJSON_AddItemToArray(arr, n2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON *refs = cJSON_CreateArray();
    cJSON_AddItemReferenceToArray(refs, arr);
    cJSON_AddItemToObject(root, "refs", refs);

    // step 2: Configure
    const char *json_text = "{\"meta\": {\"count\": 2}, \"list\": [1, 2, 3]}";
    cJSON *parsed = cJSON_Parse(json_text);
    cJSON *meta = cJSON_GetObjectItemCaseSensitive(parsed, "meta");
    cJSON *list = cJSON_GetObjectItemCaseSensitive(parsed, "list");
    cJSON_AddItemToObject(root, "external", parsed);

    // step 3: Operate and Validate
    cJSON *meta_count = cJSON_GetObjectItemCaseSensitive(meta, "count");
    cJSON *first_external = cJSON_GetArrayItem(list, 0);
    double first_val = cJSON_GetNumberValue(first_external);
    cJSON *first_copy = cJSON_Duplicate(first_external, 1);
    cJSON *first_number = cJSON_CreateNumber(first_val);
    cJSON_AddItemToObject(root, "first_of_external", first_copy);
    cJSON_AddItemToObject(root, "first_value", first_number);
    char *printed = cJSON_Print(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}