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
//<ID> 1011
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *values = NULL;
    cJSON *raw_item = NULL;
    cJSON *raw_replacement = NULL;
    cJSON *flag = NULL;
    cJSON *first_elem = NULL;
    char *printed = NULL;
    double numbers[3];
    int count = 3;
    double first_val = 0.0;
    cJSON_bool replaced = 0;
    cJSON_bool flag_true = 0;

    // step 2: Setup / Configure
    numbers[0] = 1.1;
    numbers[1] = 2.2;
    numbers[2] = 3.3;
    root = cJSON_CreateObject();
    values = cJSON_CreateDoubleArray(numbers, count);
    cJSON_AddItemToObject(root, "values", values);
    raw_item = cJSON_CreateRaw("\"initial_raw\"");
    cJSON_AddItemToObject(root, "raw", raw_item);
    flag = cJSON_CreateTrue();
    cJSON_AddItemToObject(root, "flag", flag);

    // step 3: Operate / Validate
    printed = cJSON_PrintUnformatted(root);
    raw_replacement = cJSON_CreateRaw("\"replaced_raw\"");
    replaced = cJSON_ReplaceItemViaPointer(root, raw_item, raw_replacement);
    flag_true = cJSON_IsTrue(cJSON_GetObjectItem(root, "flag"));
    first_elem = cJSON_GetArrayItem(values, 0);
    first_val = cJSON_GetNumberValue(first_elem);

    // step 4: Cleanup
    cJSON_free(printed);
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}