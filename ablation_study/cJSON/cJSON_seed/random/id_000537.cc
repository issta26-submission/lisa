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
//<ID> 537
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
    cJSON *dbl_array = (cJSON *)0;
    cJSON *raw_item = (cJSON *)0;
    cJSON *label_item = (cJSON *)0;
    cJSON *retrieved_container = (cJSON *)0;
    cJSON *retrieved_raw = (cJSON *)0;
    cJSON *retrieved_label = (cJSON *)0;
    double numbers[] = {1.5, 2.5, 3.5};
    int count = 3;
    char *label_val = (char *)0;
    cJSON_bool add_ok1 = 0;
    cJSON_bool add_ok2 = 0;
    cJSON_bool add_ok3 = 0;
    cJSON_bool add_ok4 = 0;
    cJSON_bool raw_check = 0;

    // step 2: Initialize
    root = cJSON_CreateObject();
    container = cJSON_CreateArray();
    dbl_array = cJSON_CreateDoubleArray(numbers, count);
    raw_item = cJSON_CreateRaw("raw_payload");
    label_item = cJSON_CreateString("example_label");

    // step 3: Configure
    add_ok1 = cJSON_AddItemToArray(container, dbl_array);
    add_ok2 = cJSON_AddItemToArray(container, raw_item);
    add_ok3 = cJSON_AddItemToObject(root, "items", container);
    add_ok4 = cJSON_AddItemToObject(root, "label", label_item);

    // step 4: Operate
    retrieved_container = cJSON_GetObjectItem(root, "items");
    retrieved_raw = cJSON_GetArrayItem(retrieved_container, 1);
    raw_check = cJSON_IsRaw(retrieved_raw);
    retrieved_label = cJSON_GetObjectItem(root, "label");
    label_val = cJSON_GetStringValue(retrieved_label);

    // step 5: Validate
    (void)add_ok1;
    (void)add_ok2;
    (void)add_ok3;
    (void)add_ok4;
    (void)raw_check;
    (void)label_val;

    // step 6: Cleanup
    cJSON_Delete(root);

    // API sequence test completed successfully
    return 66;
}