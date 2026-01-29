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
//<ID> 1258
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_cJSON_api_sequence() {
    // step 1: Declarations
    cJSON *root = (cJSON *)0;
    cJSON *base = (cJSON *)0;
    cJSON *container = (cJSON *)0;
    cJSON *ref = (cJSON *)0;
    cJSON *n1 = (cJSON *)0;
    cJSON *n2 = (cJSON *)0;
    cJSON_bool is_str = 0;
    int size_base = 0;
    int size_container = 0;
    int validation_score = 0;

    // step 2: Initialize - create root object, two arrays and numeric items
    root = cJSON_CreateObject();
    base = cJSON_CreateArray();
    container = cJSON_CreateArray();
    n1 = cJSON_CreateNumber(1.0);
    n2 = cJSON_CreateNumber(2.0);

    // step 3: Configure - populate base array, create an array reference to base, and attach structures to root
    cJSON_AddItemToArray(base, n1);
    cJSON_AddItemToArray(base, n2);
    ref = cJSON_CreateArrayReference(base);
    cJSON_AddItemToArray(container, ref);
    cJSON_AddItemToObject(root, "base", base);
    cJSON_AddItemToObject(root, "views", container);

    // step 4: Operate - inspect types and measure sizes
    is_str = cJSON_IsString(n1);
    size_base = cJSON_GetArraySize(base);
    size_container = cJSON_GetArraySize(container);

    // step 5: Validate - compute a simple validation score to ensure data flowed through APIs
    validation_score = (int)(root != (cJSON *)0) + (int)(base != (cJSON *)0) + (int)(container != (cJSON *)0) + (int)(ref != (cJSON *)0) + (int)(n1 != (cJSON *)0) + (int)(n2 != (cJSON *)0) + (int)is_str + size_base + size_container;
    (void)validation_score;

    // step 6: Cleanup - delete the root (which also deletes attached arrays and items)
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}