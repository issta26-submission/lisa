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
//<ID> 277
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
    cJSON *num1 = cJSON_CreateNumber(1.5);
    cJSON *num2 = cJSON_CreateNumber(42.0);
    char *json_buf = (char *)cJSON_malloc(64);
    memset(json_buf, 0, 64);
    const char *raw = " { \"a\" : 10 , \"b\" : 20 } ";
    size_t raw_len = strlen(raw);
    memcpy(json_buf, raw, raw_len + 1);

    // step 2: Setup / Configure
    cJSON_AddItemToArray(arr, num1);
    cJSON_AddItemToArray(arr, num2);
    cJSON_AddItemToObject(root, "numbers", arr);
    cJSON_Minify(json_buf);

    // step 3: Operate / Validate
    int size = cJSON_GetArraySize(arr);
    cJSON *first_item = cJSON_GetArrayItem(arr, 0);
    double first_value = cJSON_GetNumberValue(first_item);
    cJSON_AddItemToObject(root, "first_value", cJSON_CreateNumber(first_value));
    (void)size;

    // step 4: Cleanup
    cJSON_free(json_buf);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}