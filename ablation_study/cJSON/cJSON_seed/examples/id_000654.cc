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
//<ID> 654
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Initialize
    const char *version = cJSON_Version();
    cJSON *root = cJSON_CreateObject();
    double numbers[3] = {1.1, 2.2, 3.3};
    cJSON *dbl_array = cJSON_CreateDoubleArray(numbers, 3);
    cJSON *original_key = cJSON_CreateString("initial");

    // step 2: Configure
    cJSON_AddItemToObject(root, "values", dbl_array);
    cJSON_AddItemToObject(root, "key", original_key);
    cJSON_AddStringToObject(root, "meta", "tester");

    // step 3: Operate & Validate
    char *snapshot = cJSON_PrintUnformatted(root);
    cJSON_Minify(snapshot);
    cJSON *parsed = cJSON_Parse(snapshot);
    cJSON *parsed_values = cJSON_GetObjectItem(parsed, "values");
    int array_size = cJSON_GetArraySize(parsed_values);
    cJSON *size_item = cJSON_CreateNumber((double)array_size);
    cJSON_ReplaceItemInObjectCaseSensitive(root, "key", size_item);
    (void)version;
    (void)array_size;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}