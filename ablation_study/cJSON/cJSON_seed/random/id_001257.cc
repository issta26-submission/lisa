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
//<ID> 1257
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *container = (cJSON *)0;
    cJSON *num_child = (cJSON *)0;
    cJSON *arrref = (cJSON *)0;
    cJSON *str_item = (cJSON *)0;
    int size_before = 0;
    int size_after = 0;
    cJSON_bool str_is_string = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, an array container, a numeric child and a string item
    root = cJSON_CreateObject();
    container = cJSON_CreateArray();
    num_child = cJSON_CreateNumber(123.456);
    str_item = cJSON_CreateString("example");

    // step 3: Configure - create an array reference to the numeric child and add items to the container
    arrref = cJSON_CreateArrayReference((const cJSON *)num_child);
    cJSON_AddItemToArray(container, arrref);
    cJSON_AddItemToArray(container, str_item);
    cJSON_AddItemToObject(root, "payload", container);

    // step 4: Operate - inspect the container size and check the string item type
    size_before = cJSON_GetArraySize(container);
    str_is_string = cJSON_IsString(str_item);
    size_after = cJSON_GetArraySize(container);

    // step 5: Validate - compute a small validation score to ensure flow through APIs
    validation_score = (int)(root != (cJSON *)0) + (int)(container != (cJSON *)0) + (int)(num_child != (cJSON *)0) + (int)(arrref != (cJSON *)0) + size_before + size_after + (int)str_is_string;
    (void)validation_score;

    // step 6: Cleanup - delete root (frees container, arrref and str_item), then delete the separately referenced numeric child
    cJSON_Delete(root);
    cJSON_Delete(num_child);

    // API sequence test completed successfully
    return 66;
}