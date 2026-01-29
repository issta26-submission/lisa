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
//<ID> 1314
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *obj = NULL;
    cJSON *arr = NULL;
    cJSON *num1 = NULL;
    cJSON *str1 = NULL;
    cJSON *item_raw = NULL;
    cJSON *duplicate = NULL;
    cJSON *name_item = NULL;
    char *printed = NULL;
    const char *parse_end = NULL;
    cJSON_bool is_raw = 0;
    cJSON_bool equal = 0;
    static const char json[] = "{\"name\":\"value\",\"raw\":123, \"nested\":{\"a\":1,\"b\":2}}";
    size_t len = sizeof(json) - 1;

    // step 2: Setup / Configure
    root = cJSON_ParseWithLengthOpts(json, len, &parse_end, 1);
    obj = cJSON_GetObjectItem(root, "nested");
    arr = cJSON_AddArrayToObject(root, "new_array");
    num1 = cJSON_CreateNumber(3.14);
    cJSON_AddItemToArray(arr, num1);
    str1 = cJSON_CreateString("hello");
    cJSON_AddItemToArray(arr, str1);

    // step 3: Operate / Validate
    item_raw = cJSON_GetObjectItem(root, "raw");
    is_raw = cJSON_IsRaw(item_raw);
    duplicate = cJSON_Duplicate(obj, 1);
    equal = cJSON_Compare(obj, duplicate, 1);
    name_item = cJSON_GetObjectItem(root, "name");
    printed = cJSON_PrintUnformatted(root);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    cJSON_Delete(duplicate);
    // API sequence test completed successfully
    return 66;
}