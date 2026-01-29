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
//<ID> 1169
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *parsed = NULL;
    cJSON *numbers_array = NULL;
    cJSON *first_item = NULL;
    cJSON *root = NULL;
    cJSON *copied_number = NULL;
    cJSON *flag_true = NULL;
    cJSON *flag_false = NULL;
    char *printed = NULL;
    int array_size = 0;
    double first_value = 0.0;
    double validated_value = 0.0;
    char json[] = "{\"numbers\":[1,2,3],\"value\":42}";

    // step 2: Setup / Configure
    parsed = cJSON_Parse(json);
    numbers_array = cJSON_GetObjectItem(parsed, "numbers");
    array_size = cJSON_GetArraySize(numbers_array);
    first_item = cJSON_GetArrayItem(numbers_array, 0);
    first_value = cJSON_GetNumberValue(first_item);
    root = cJSON_CreateObject();
    copied_number = cJSON_CreateNumber(first_value);
    flag_true = cJSON_CreateTrue();
    flag_false = cJSON_CreateFalse();
    cJSON_AddItemToObject(root, "first", copied_number);
    cJSON_AddItemToObject(root, "count", cJSON_CreateNumber((double)array_size));
    cJSON_AddItemToObject(root, "ok", flag_true);
    cJSON_AddItemToObject(root, "no", flag_false);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    validated_value = cJSON_GetNumberValue(cJSON_GetObjectItem(root, "first"));
    (void)validated_value;
    (void)printed;

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(parsed);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}