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
//<ID> 675
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
    cJSON *array = cJSON_CreateArray();
    cJSON *num1 = cJSON_CreateNumber(42.0);
    cJSON *num2 = cJSON_CreateNumber(7.5);
    cJSON *num_ref = cJSON_CreateNumber(100.0);

    // step 2: Configure
    cJSON_AddItemToArray(array, num1);
    cJSON_AddItemToArray(array, num2);
    cJSON_AddItemReferenceToArray(array, num_ref);
    cJSON_AddItemToObject(root, "data", array);
    cJSON *count_item = cJSON_AddNumberToObject(root, "count", 3.0);
    double set_result = cJSON_SetNumberHelper(count_item, 3.0);

    // step 3: Operate & Validate
    char *snapshot = cJSON_Print(root);
    size_t len = strlen(snapshot);
    char *buffer = (char *)cJSON_malloc(len + 1);
    memset(buffer, 0, len + 1);
    memcpy(buffer, snapshot, len + 1);
    cJSON_Minify(buffer);
    cJSON *parsed = cJSON_Parse(buffer);
    cJSON *parsed_array = cJSON_GetObjectItemCaseSensitive(parsed, "data");
    int parsed_size = cJSON_GetArraySize(parsed_array);
    cJSON *first_item = cJSON_GetArrayItem(parsed_array, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    (void)version;
    (void)set_result;
    (void)parsed_size;
    (void)first_value;

    // step 4: Cleanup
    cJSON_free(snapshot);
    cJSON_free(buffer);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}