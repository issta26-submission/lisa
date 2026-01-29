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
//<ID> 1474
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations / Initialize
    cJSON *root = NULL;
    cJSON *numbers = NULL;
    cJSON *num1 = NULL;
    cJSON *num2 = NULL;
    cJSON *meta = NULL;
    cJSON *name_item = NULL;
    cJSON *numbers_ref = NULL;
    cJSON *second_item = NULL;
    cJSON *dup_second = NULL;
    cJSON *name_ref = NULL;
    cJSON *detached_meta = NULL;
    double dup_value = 0.0;
    const char *name_str = NULL;

    // step 2: Setup / Configure
    root = cJSON_CreateObject();
    numbers = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "numbers", numbers);
    num1 = cJSON_CreateNumber(10.0);
    num2 = cJSON_CreateNumber(20.0);
    cJSON_AddItemToArray(numbers, num1);
    cJSON_AddItemToArray(numbers, num2);
    meta = cJSON_CreateObject();
    name_item = cJSON_CreateString("example");
    cJSON_AddItemToObject(meta, "name", name_item);
    cJSON_AddItemToObject(root, "meta", meta);

    // step 3: Operate / Validate
    numbers_ref = cJSON_GetObjectItem(root, "numbers");
    second_item = cJSON_GetArrayItem(numbers_ref, 1);
    dup_second = cJSON_Duplicate(second_item, 1);
    meta = cJSON_GetObjectItem(root, "meta");
    name_ref = cJSON_GetObjectItem(meta, "name");
    detached_meta = cJSON_DetachItemViaPointer(root, meta);
    dup_value = cJSON_GetNumberValue(dup_second);
    name_str = cJSON_GetStringValue(name_ref);
    (void)dup_value;
    (void)name_str;

    // step 4: Cleanup
    cJSON_Delete(dup_second);
    cJSON_Delete(detached_meta);
    cJSON_Delete(root);
    // API sequence test completed successfully
    return 66;
}