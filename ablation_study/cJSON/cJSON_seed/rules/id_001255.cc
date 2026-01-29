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
//<ID> 1255
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    const double numbers[] = {1.5, 2.5, 3.5};
    int numbers_count = 3;
    cJSON *root = NULL;
    cJSON *values = NULL;
    cJSON *second_item = NULL;
    cJSON *null_item = NULL;
    cJSON *str_item = NULL;
    cJSON *raw_item = NULL;
    const char *raw_json = "{\"inner\":true}";
    char *printed = NULL;
    double second_value = 0.0;
    cJSON *tmp = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    values = cJSON_CreateDoubleArray(numbers, numbers_count);
    cJSON_AddItemToObject(root, "values", values);
    null_item = cJSON_AddNullToObject(root, "nothing");
    str_item = cJSON_AddStringToObject(root, "greeting", "hello");
    raw_item = cJSON_AddRawToObject(root, "meta", raw_json);

    // step 3: Operate / Validate
    tmp = cJSON_GetObjectItem(root, "values");
    second_item = cJSON_GetArrayItem(tmp, 1);
    second_value = cJSON_GetNumberValue(second_item);
    cJSON_AddNumberToObject(root, "second_value", second_value);
    printed = cJSON_PrintUnformatted(root);
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}